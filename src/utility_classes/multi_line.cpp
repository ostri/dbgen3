#include "common.hpp"
#include "multi_line.hpp"
#include "string_format.hpp"

namespace dbgen3
{

  str_vec multi_line::str_to_vec(cstr_t a_str)
  {
    str_vec r;
    auto    o(rtrim(a_str)); /// remove trailing whitespaces
    r = split(o, '\n');
    if (! r.empty() && rtrim(r[0]).empty()) // trim leading empty lines
      r.erase(r.begin());
    r = trim_left(r);
    return r;
  }
  str_vec multi_line::trim_left(const str_vec& o)
  {
    const std::string WHITESPACE = " \n\r\t\f\v";
    auto              min        = o.size() ? o.at(0).size() : 0;
    if (min > 0)
    {
      for (auto line : o)
      {
        auto start = line.find_first_not_of(WHITESPACE);
        min        = start < min ? start : min;
        if (min == 0) return o; // no possibillity to trim
      }
      str_vec r;
      /// all lines have at least "min" leading whitespaces ltrim it
      for (auto line : o) { r.push_back(line.substr(min)); };
      return r;
    }
    else return o;
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