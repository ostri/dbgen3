
#ifndef SERIALIZE_TEMPL_HPP
#define SERIALIZE_TEMPL_HPP

#include <span>
#include <sqlcli1.h>
#include <stdexcept>
#include <string>
#include <string_view>

namespace db
{
  using bstr_t  = std::span<uint8_t>;
  using cbstr_t = std::span<const uint8_t>;
  using cstr_t  = std::string_view;
  template <typename T>
  inline std::string serialize(const T& val)
  {
    if constexpr (! std::is_class<T>::value)
    {
      std::string s;
      s += "'" + std::to_string(val) + "'";
      return s;
    }
    else throw std::runtime_error("fishy");
  }
  template <>
  inline std::string serialize<cbstr_t>(const cbstr_t& val)
  {
    std::string       s("'");
    static const auto tbl       = std::to_array("0123456789abcdef");
    const uint8_t     high_mask = 0xf0;
    const uint8_t     low_mask  = 0x0f;
    for (auto el : val)
    {
      uint8_t high = high_mask & el;
      uint8_t low  = low_mask & el;
      s += tbl[high]; // NOLINT clang-tidy(cppcoreguidelines-pro-bounds-constant-array-index)
      s += tbl[low];  // NOLINT clang-tidy(cppcoreguidelines-pro-bounds-constant-array-index)
      s += ' ';
    }
    s.resize(s.size() - 1); // remove trailing space
    s += "'";
    return s;
  }
  template <>
  inline std::string serialize<cstr_t>(const cstr_t& val)
  {
    std::string s;
    s += "'" + std::string(val) + "'";
    return s;
  }
  template <>
  inline std::string serialize<TIMESTAMP_STRUCT>(const TIMESTAMP_STRUCT& val)
  {
    std::string s;
    // clang-format off
    s += "'" +
    std::to_string(val.year) + "-" + std::to_string(val.month) + "-" + std::to_string(val.day) + " " +
    std::to_string(val.hour) + ":" +
    std::to_string(val.minute) + ":" +
    std::to_string(val.second) + "." +
    std::to_string(val.fraction) + "'"
    ;
    // clang-format on
    return s;
  }
  template <>
  inline std::string serialize<SQL_DATE_STRUCT>(const SQL_DATE_STRUCT& val)
  {
    std::string s;
    // clang-format off
    s += "'" +
    std::to_string(val.year) + "-" + std::to_string(val.month) + "-" + std::to_string(val.day) + "'"
    ;
    // clang-format on
    return s;
  }
  template <>
  inline std::string serialize<SQL_TIME_STRUCT>(const SQL_TIME_STRUCT& val)
  {
    std::string s;
    // clang-format off
    s += "'" +
    std::to_string(val.hour) + ":" +
    std::to_string(val.minute) + ":" +
    std::to_string(val.second) + "'"
    ;
    // clang-format on
    return s;
  }
};     // namespace db
#endif // SERIALIZE_TEMPL_HPP
