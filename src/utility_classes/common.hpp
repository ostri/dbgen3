#ifndef guard__COMMON_HPP_QSN7IE6JX4HO__
#define guard__COMMON_HPP_QSN7IE6JX4HO__
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "log.hpp"
namespace dbgen3
{
  ///////////////////////////////////////////////////////////
  /// @brief common conts, functions and types
  ///

  using cstr_t  = std::string_view;
  using str_t   = std::string;
  using str_vec = std::vector<std::string>;

  /**
 * @brief fetch token id
 *
 * The method returns the token id from the token set or 0
 * if not found.
 *
 * @param token     token which index we are looking for
 * @param token_set set of tokens that we are searching in
 * @return
 */
  int g_token_ndx(const str_t token, const str_vec& token_set);
  /**
 * @brief it returns string associated with the token id from the token set.
 *
 * @param token_id token id that searching for string
 * @param token_set set of tokens
 * @return string version of the token id
 */
  const str_t g_token_str(int token_id, const str_vec& token_set);
} // namespace dbgen3
#endif // guard__COMMON_HPP_QSN7IE6JX4HO__
