#ifndef COMMON_HPP
#define COMMON_HPP
#include <glog/logging.h>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include <sys/stat.h>

#include "log.hpp"
#include "program_status.hpp"

namespace dbgen3
{
  ///////////////////////////////////////////////////////////
  /// @brief common conts, functions and types
  ///

  using cstr_t  = std::string_view;
  using str_t   = std::string;
  using str_vec = std::vector<std::string>;

  const uint align_64 = 64; //!< align to 64 byte boundry

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
  int g_token_ndx(const str_t& token, const str_vec& token_set);
  /**
   * @brief it returns string associated with the token id from the token set.
   *
   * @param token_id token id that searching for string
   * @param token_set set of tokens
   * @return string version of the token id
   */
  const str_t& g_token_str(int token_id, const str_vec& token_set);

  /**
   * @brief test whether the provided path/file exists
   *
   * @param path path we want to check whether exists or not
   * @return true - the provided path exists
   *         false - the provided path does not exist
   */
  inline bool file_exists(const str_t& path)
  {
    struct stat buffer
    {
    };
    return (stat(path.c_str(), &buffer) == 0);
  }
} // namespace dbgen3
#endif // COMMON_HPP
