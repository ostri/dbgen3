#include "common.hpp"
#include <cassert>
#include <iostream>
namespace dbgen3
{
  int g_token_ndx(const str_t& token, const str_vec& token_set)
  {
#pragma unroll 20
    for (auto ndx = 0; ndx < token_set.size(); ++ndx)
    {
      if (token_set.at(ndx) == token) { return ndx; }
    }
    return 0;
  }
  const str_t& g_token_str(int token_id, const str_vec& token_set)
  {
    assert(token_id >= 0);               // NOLINT
    assert(token_id < token_set.size()); // NOLINT
    return token_set.at(token_id);
  }
} // namespace dbgen3
