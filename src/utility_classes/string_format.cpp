#include "string_format.hpp"
namespace dbgen3
{
  str_t string_format::ln() { return s("\n", 0); }

  str_t string_format::s(const str_t& a_msg, uint8_t offs)
  {
    return std::string(offs, ' ') + a_msg;
  }

  str_t string_format::sl(const str_t& a_msg, uint8_t offs)
  {
    return s(a_msg, offs) + ln();
  }

  str_t string_format::s(const str_t& a_prompt,
                         const str_t& a_msg,
                         uint8_t      offs)
  {
    return std::string(offs, ' ') + a_prompt + a_msg;
  }

  str_t string_format::sl(const str_t& a_prompt,
                          const str_t& a_msg,
                          uint8_t      offs)
  {
    return s(a_prompt, a_msg, offs) + ln();
  }

  // str_t string_format::s(cstr_t a_msg, uint8_t offs)
  // {
  //   return offs != 0
  //            ? std::string(offs, ' ') + std::string(a_msg.data(), a_msg.size())
  //            : std::string(a_msg.data(), a_msg.size());
  // }

  // str_t string_format::sl(cstr_t a_msg, uint8_t offs)
  // {
  //   return s(a_msg, offs) + ln();
  // }

  // str_t string_format::s(cstr_t a_prompt, cstr_t a_msg, uint8_t offs)
  // {
  //   return std::string(offs, ' ') + s(a_prompt) + s(a_msg);
  // }

  // str_t string_format::sl(cstr_t a_prompt, cstr_t a_msg, uint8_t offs)
  // {
  //   return s(a_prompt, a_msg, offs) + ln();
  // }
} // namespace dbgen3
