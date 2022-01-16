#ifndef COMMON_HPP
#define COMMON_HPP
#include <glog/logging.h>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/core.h>
#include <sys/stat.h>
#include <fmt/ranges.h>
#include <fmt/color.h>
#include <fmt/format.h>

#include "log.hpp"
//#include "program_status.hpp"
//#include "enums.hpp"

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
  uint g_token_ndx(const str_t& token, const str_vec& token_set);
  /**
   * @brief it returns string associated with the token id from the token set.
   *
   * @param token_id token id that searching for string
   * @param token_set set of tokens
   * @return string version of the token id
   */
  const str_t& g_token_str(uint token_id, const str_vec& token_set);
  /**
   * @brief split the provided string into vector of stringa
   *
   * @param s string to be decomposed
   * @param delim split point
   * @return str_vec
   */
  str_vec split(const std::string& s, char delim);
  /**
   * @brief test whether the provided path/file exists
   *
   * @param path path we want to check whether exists or not
   * @return true - the provided path exists
   *         false - the provided path does not exist
   */
  inline bool file_exists(const str_t& path)
  {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
  }

  inline std::string ctx_to_str(str_vec a_ctx, cstr_t last)
  {
    const str_vec prompts = {"q-set", "q"};
    std::string r;
    for (auto cnt=0UL; cnt < a_ctx.size(); ++cnt)
    {
      if (cnt < prompts.size())
        r += fmt::format("/{}:{}",prompts[cnt], a_ctx[cnt]);
        else 
        r += fmt::format("/:{}", a_ctx[cnt]);
    }
    r += std::string("/") + std::string(last);
    return r;
  }
} // namespace dbgen3
#endif // COMMON_HPP
