#include "common.hpp"
#include <cassert>
#include <iostream>
namespace dbgen3
{
  /**
   * @brief split the provided string at delimiters
   *
   * @param s string to be separated
   * @param delim token separator/delimiter
   * @return str_vec vector of tokens
   */
  str_vec split(cstr_t s, char delim)
  {
    str_vec           r;
    std::stringstream ss(s.data());
    std::string       item;
    while (std::getline(ss, item, delim)) { r.push_back(item); }
    return r;
  }
  str_t snake_case(cstr_t a_name)
  {
    str_t r;
    for (auto ch : a_name)
      if (ch == '-') r += '_';
      else r += ch;
    return r;
  }

} // namespace dbgen3
