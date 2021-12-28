#ifndef guard__STRING_FORMAT_HPP_SQYE15VK0LXH__
#define guard__STRING_FORMAT_HPP_SQYE15VK0LXH__
#include "common.hpp"
namespace dbgen3 {
///////////////////////////////////////////////////////////
/// @brief string_format
///
class string_format {
public:
  string_format() = default;
  ~string_format() = default;
  string_format(const string_format &other) = default;
  string_format(string_format &&other) = default;
  string_format &operator=(const string_format &other) = default;
  string_format &operator=(string_format &&other) = default;

  static str_t ln();
  static str_t s(str_t a_msg, uint8_t offs = 0);
  static str_t sl(str_t a_msg, uint8_t offs = 0);
  static str_t s(str_t a_prompt, str_t a_msg, uint8_t offs = 0);
  static str_t sl(str_t a_prompt, str_t a_msg, uint8_t offs = 0);

  static str_t s(cstr_t a_msg, uint8_t offs = 0);
  static str_t sl(cstr_t a_msg, uint8_t offs = 0) {
    return s(a_msg, offs) + ln();
  }
  static str_t s(cstr_t a_prompt, cstr_t a_msg, uint8_t offs = 0);
  static str_t sl(cstr_t a_prompt, cstr_t a_msg, uint8_t offs = 0);

  bool operator==(const string_format &other) const;
  bool operator!=(const string_format &other) const;

private:
  // Members
};
using out = string_format;
} // namespace dbgen3
#endif // guard__STRING_FORMAT_HPP_SQYE15VK0LXH__
