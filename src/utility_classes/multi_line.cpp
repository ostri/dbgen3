#include <cctype>
#include <span>

#include "common.hpp"
#include "multi_line.hpp"
#include "string_format.hpp"

namespace dbgen3
{

  str_vec multi_line::to_vec(cstr_t a_str)
  {
    str_vec r;
    r = minimize(split(rtrim(a_str), '\n'));
    return r;
  }
  bool multi_line::is_whitespace_line(cstr_t l)
  { // NOLINTNEXTLINE clang-tidy(readability-use-anyofallof)
    for (const auto& ch : l)
      if (std::isspace(ch) == 0) return false;
    return true;
  }
  /**
   * @brief finds first non ws line in the provided block
   *
   * @param o block of lines
   * @return int index of first non WS or size of the block if all empty
   */
  std::size_t multi_line::find_first_non_WS_line(const str_vec& o)
  {
    if (o.empty()) return 0;
    auto r = 0UL;
    while ((r < o.size()) && is_whitespace_line(o[r])) ++r;
    return r;
  }
  /**
   * @brief finds last non ws line in the provided block
   *
   * @param o block of lines
   * @return int index of last non WS line or -1 if all WS
   */
  int64_t multi_line::find_last_non_WS_line(const str_vec& o)
  {
    if (o.empty()) return -1;
    auto r = static_cast<int64_t>(o.size() - 1);
    while ((r >= 0) && is_whitespace_line(o[r])) --r;
    return r;
  }
  /**
   * @brief minimize provided block of lines
   *
   * The method eliminates leading / trailing empty lines (i.e. all whitespace) and
   * moves all remaining lines n characters to the left perserving structure.
   *
   * @param o block of lines
   * @return multi_line::str_vec
   */
  str_vec multi_line::minimize(const str_vec& o)
  {
    if (! o.empty())
    { /* block of lines is non empty */
      auto first = find_first_non_WS_line(o);
      if (o.size() == static_cast<std::size_t>(first)) return {}; // all lines are WS
      auto                   last = find_last_non_WS_line(o);
      std::span<const str_t> block{&o[first], static_cast<std::size_t>(last - first + 1)};

      auto min = block.data()->size();
      for (const auto& l : block)
      {
        if (! is_whitespace_line(l))
        { // we can have embeded WS lines
          auto pos = 0UL;
          while (isspace(l[pos]) != 0) ++pos; // nonempty line, we do need guard
          if (pos < min) min = pos;
        }
      };
      str_vec r; // allocate enough space
      r.reserve(block.size());
      for (const auto& l : block)
      {
        if (l.size() >= min) r.push_back(l.substr(min));
        else r.push_back(""); // embeded ws line
      }
      return r;
      // const std::string WHITESPACE = " \n\r\t\f\v";
      // auto              min        = ! o.empty() ? o.at(0).size() : 0;
      // if (min > 0)
      // {
      //   for (const auto& line : o)
      //   {
      //     auto start = std::min(line.size(), line.find_first_not_of(WHITESPACE));
      //     min        = start < min ? start : min;
      //     if (min == 0) return o; // no possibillity to trim
      //   }
      //   str_vec r;
      //   /// all lines have at least "min" leading whitespaces ltrim it
      //   for (const auto& line : o) { r.push_back(line.substr(min)); };
      //   return r;
      // }
    }
    return o;
  }

  // str_vec multi_line::trim_left(const str_vec& o)
  // {
  //   const std::string WHITESPACE = " \n\r\t\f\v";
  //   auto              min        = ! o.empty() ? o.at(0).size() : 0;
  //   if (min > 0)
  //   {
  //     for (const auto& line : o)
  //     {
  //       auto start = std::min(line.size(), line.find_first_not_of(WHITESPACE));
  //       min        = start < min ? start : min;
  //       if (min == 0) return o; // no possibillity to trim
  //     }
  //     str_vec r;
  //     /// all lines have at least "min" leading whitespaces ltrim it
  //     for (const auto& line : o) { r.push_back(line.substr(min)); };
  //     return r;
  //   }
  //   return o;
  // }
  multi_line::multi_line(str_vec o)
  : lines_(std::move(o))
  { }
  multi_line::multi_line(cstr_t a_str)
  : lines_(to_vec(a_str))
  { }
  multi_line::multi_line(const std::string& a_str)
  : lines_(to_vec(a_str))
  { }
  //  multi_line&    operator=(multi_line&&) = default;
  multi_line& multi_line::operator=(const std::string& o)
  {
    lines_ = to_vec(o);
    return *this;
  }
  multi_line::operator std::string() const
  {
    std::string r;
    for (const auto& l : lines_) r += str_t(trim(l)) + ' ';
    return r;
  }
  std::string multi_line::dump() const { return dump("", 0); }
  std::string multi_line::dump(uint offs) const { return dump("", offs); }

  std::string multi_line::dump(cstr_t o, uint offs) const
  {
    std::string s(o);
    // s += std::to_string(lines_.size());
    for (const auto& l : lines_) s += out::sl(l, offs);
    return s;
  }
}; // namespace dbgen3