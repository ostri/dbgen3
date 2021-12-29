#ifndef STRING_FORMAT_HPP
#define STRING_FORMAT_HPP

#include "common.hpp"
namespace dbgen3
{
  class string_format;
  using out = string_format;
  ///////////////////////////////////////////////////////////
  /// @brief string_format
  ///
  class string_format
  {
  public:
    string_format()                           = default;
    ~string_format()                          = default;
    string_format(const string_format& other) = default;
    string_format(string_format&& other)      = default;
    string_format& operator=(const string_format& other) = default;
    string_format& operator=(string_format&& other) = default;

    static str_t ln();
    static str_t s(const str_t& a_msg, uint8_t offs);
    static str_t sl(const str_t& a_msg, uint8_t offs = 0);
    static str_t s(const str_t& a_prompt, const str_t& a_msg, uint8_t offs = 0);
    static str_t sl(const str_t& a_prompt, const str_t& a_msg, uint8_t offs);

    // static str_t s(cstr_t a_msg, uint8_t offs);
    // static str_t sl(cstr_t a_msg, uint8_t offs = 0);
    // static str_t s(cstr_t a_prompt, cstr_t a_msg, uint8_t offs = 0);
    // static str_t sl(cstr_t a_prompt, cstr_t a_msg, uint8_t offs);
  private:
    // Members
  };
  using out = string_format;
} // namespace dbgen3
#endif // STRING_FORMAT_HPP
