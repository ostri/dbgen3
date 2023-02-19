#ifndef COMMON_HPP
#define COMMON_HPP
#include <algorithm>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <glog/logging.h>
#include <iostream>
#include <string>
#include <string_view>
#include <sys/stat.h>
#include <vector>

#include "log.hpp"

#if defined(__clang__) || defined(__GNUC__)
#  define ATTRIBUTE_NO_SANITIZE_ADDRESS __attribute__((no_sanitize_address))
#else
#  define ATTRIBUTE_NO_SANITIZE_ADDRESS
#endif
namespace dbgen3
{
  ///////////////////////////////////////////////////////////
  /// @brief common conts, functions and types
  ///

  using cstr_t  = std::string_view;
  using str_t   = std::string;
  using str_vec = std::vector<std::string>;

  const uint align_64 = 64; //!< align to 64 byte boundry

  std::string ctx_to_str(const str_vec& a_ctx);
  std::string ctx_to_str(const str_vec& a_ctx, cstr_t last);
  bool        file_exists(cstr_t path);
  str_t       snake_case(cstr_t a_name);


  // rtrim
  inline cstr_t rtrim(cstr_t o)
  {
    // str_t s(o.begin(), std::find_if(o.rbegin(), o.rend(), [](unsigned char ch) { return
    // std::isspace(ch) == 0; })
    //           .base()-1);
    return {o.begin(),
            std::find_if(o.rbegin(), o.rend(), [](unsigned char ch1) { return std::isspace(ch1) == 0; }).base()};
  }
  // ltrim
  inline cstr_t ltrim(cstr_t o)
  {
    // std::string s(o);
    // auto        pos = std::find_if_not(o.begin(), o.end(), [](char ch) { return isspace(ch); });
    // s.erase(s.begin(), pos);
    return {std::find_if_not(o.begin(), o.end(), [](char ch1) { return isspace(ch1); }), o.end()};
  }
  inline cstr_t trim(cstr_t o) { return ltrim(rtrim(o)); }
  /**
   * @brief split the provided string into vector of stringa
   *
   * @param s string to be decomposed
   * @param delim split point
   * @return str_vec
   */
  str_vec split(cstr_t s, char delim);
  /**
   * @brief test whether the provided path/file exists
   *
   * @param path path we want to check whether exists or not
   * @return true - the provided path exists
   *         false - the provided path does not exist
   */
  inline bool file_exists(cstr_t path)
  {
    struct stat buffer
    { };
    return (stat(std::string(path).data(), &buffer) == 0);
  }

  inline std::string ctx_to_str(const str_vec& a_ctx) { return ctx_to_str(a_ctx, ""); }
  inline std::string ctx_to_str(const str_vec& a_ctx, cstr_t last)
  {
    const str_vec prompts = {"q-set", "q"}; // NOLINT cppcoreguidelines-init-variables
    std::string   r{};
    for (auto cnt = 0UL; cnt < a_ctx.size(); ++cnt)
    {
      if (cnt < prompts.size()) r += fmt::format("/{}:{}", prompts[cnt], a_ctx[cnt]);
      else r += fmt::format("/:{}", a_ctx[cnt]);
    }
    if (! last.empty()) r += std::string("/") + std::string(last);
    return r;
  }

} // namespace dbgen3
#endif // COMMON_HPP
