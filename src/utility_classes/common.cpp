#include "common.hpp"
#include <cassert>
#include <iostream>
namespace dbgen3
{
  // uint g_token_ndx(const str_t& token, const str_vec& token_set)
  // {
  //   for (auto ndx = 0UL; ndx < token_set.size(); ++ndx)
  //     {
  //       if (token_set.at(ndx) == token) { return ndx; }
  //     }
  //   return 0;
  // }
  // const str_t& g_token_str(uint token_id, const str_vec& token_set)
  // {
  //   assert(token_id < token_set.size()); // NOLINT
  //   return token_set.at(token_id);
  // }

  str_vec split(const std::string& s, char delim)
  {
    str_vec           r;
    std::stringstream ss(s);
    std::string       item;
    while (std::getline(ss, item, delim)) { r.push_back(item); }
    return r;
  }
} // namespace dbgen3
