#include "common.hpp"
#include "multi_line.hpp"
#include "string_format.hpp"

namespace dbgen3
{

  str_vec multi_line::str_to_vec(cstr_t a_str)
  {
    str_vec r;
    r = split(rtrim(a_str), '\n');
    if (! r.empty() && rtrim(r[0]).empty()) // trim leading empty lines
      r.erase(r.begin());
    r = trim_left(r);
    return r;
  }
  str_vec multi_line::trim_left(const str_vec& o)
  {
    const std::string WHITESPACE = " \n\r\t\f\v";
    auto              min        = !o.empty() ? o.at(0).size() : 0;
    if (min > 0)
    {
      for (const auto& line : o)
      {
        auto start = std::min(line.size(), line.find_first_not_of(WHITESPACE));
        min        = start < min ? start : min;
        if (min == 0) return o; // no possibillity to trim
      }
      str_vec r;
      /// all lines have at least "min" leading whitespaces ltrim it
      for (const auto& line : o) { r.push_back(line.substr(min)); };
      return r;
    }
    return o;
  }
  multi_line::multi_line(str_vec o)
  : lines_(std::move(o))
  { 
  }
  multi_line::multi_line(cstr_t a_str)
  : lines_(str_to_vec(a_str))
  { }
  multi_line::multi_line(const std::string& a_str)
  : lines_(str_to_vec(a_str))
  {
  }
  //  multi_line&    operator=(multi_line&&) = default;
  multi_line& multi_line::operator=(const std::string& o)
  {
    lines_ = str_to_vec(o);
    return *this;
  }
  multi_line::operator std::string () const 
  {
    std::string r;
    for(const auto& l: lines_) r += str_t(trim(l))+' ';
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