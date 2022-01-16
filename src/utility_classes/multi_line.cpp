#include "multi_line.hpp"
#include "string_format.hpp"

namespace dbgen3
{
  // rtrim
  static inline std::string rtrim(cstr_t o)
  {
    std::string s(o);
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return ! std::isspace(ch); })
              .base(),
            s.end());
    return s;
  }
  str_vec multi_line::str_to_vec(cstr_t a_str)
  {
    str_vec r;
    auto o(rtrim(a_str)); /// remove trailing whitespaces
    r = split(o, '\n');
    if (! r.empty() && rtrim(r[0]).empty()) // trim leading empty lines
      r.erase(r.begin());
    return trim_left(r);
  }
  // /**
  //  * @brief Construct a new dbgen3::multi line::multi line object
  //  *
  //  * @param ml string to be converted into multiline
  //  */
  // dbgen3::multi_line::multi_line(cstr_t ml)
  // {
  //   auto o(rtrim(ml)); /// remove trailing whitespaces
  //   lines_ = split(o, '\n');
  //   if (! lines_.empty() && rtrim(lines_[0]).empty()) // trim leading empty lines
  //     lines_.erase(lines_.begin());
  //   lines_ = trim_left(lines_);
  // }
  str_vec multi_line::trim_left(const str_vec& o)
  {
    const std::string WHITESPACE = " \n\r\t\f\v";
    auto              min        = o.size() ? o.at(0).size() : 0;
    for (auto line : o)
    {
      auto start = line.find_first_not_of(WHITESPACE);
      min        = start < min ? start : min;
    }
    str_vec r;
    /// all lines have at least "min" leading whitespaces
    if (min > 0) /// only if we can trim leading whitespaces
      for (auto line : o) { r.push_back(line.substr(min)); };
    return r;
  }
  std::string multi_line::dump() const { return dump("", 0); }
  std::string multi_line::dump(int offs) const { return dump("", offs); }

  std::string multi_line::dump(cstr_t o, int offs) const
  {
    std::string s(o);
    // s += std::to_string(lines_.size());
    for (auto l : lines_) s += out::sl(l, offs);
    return s;
  }
} // namespace dbgen3