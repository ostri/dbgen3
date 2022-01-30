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

  std::string ctx_to_str(const str_vec a_ctx);
  std::string ctx_to_str(const str_vec a_ctx, cstr_t last);
  bool        file_exists(cstr_t path);

  // rtrim
  inline std::string rtrim(cstr_t o)
  {
    std::string s(o);
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return ! std::isspace(ch); })
              .base(),
            s.end());
    return s;
  }
  // ltrim
  inline std::string ltrim(cstr_t o)
  {
    std::string s(o);
    auto        pos = std::find_if_not(s.begin(), s.end(), [](char ch) { return isspace(ch); });
    s.erase(s.begin(), pos);
    return s;
  }
  inline std::string trim(cstr_t o) { return ltrim(rtrim(o)); }
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
  inline bool file_exists(cstr_t path)
  {
    struct stat buffer;
    return (stat(std::string(path).data(), &buffer) == 0);
  }

  inline std::string ctx_to_str(const str_vec a_ctx) { return ctx_to_str(a_ctx, ""); }
  inline std::string ctx_to_str(const str_vec a_ctx, cstr_t last)
  {
    const str_vec prompts = {"q-set", "q"};
    std::string   r;
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
