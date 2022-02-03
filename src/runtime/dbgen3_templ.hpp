#ifndef DBGEN3_TEMPL_HPP
#define DBGEN3_TEMPL_HPP

#include <array>
#include <string>
#include <string_view>
#include <cstring>
#include <span>
#include <iostream>
#include <sqlcli1.h>
#include <sstream>
#include <map>

#include "odbc_db.hpp"
namespace db
{
  using cstr_t = std::string_view;
  using str_t  = std::string;

  /*....atomic...............................................................................*/
  template <typename T, int DB_TYPE, int DEC = 0, std::size_t arr_size = 1>
  class atomic
  {
  public:
    using t_vec        = std::array<T, arr_size>;
    using l_vec        = std::array<int32_t, arr_size>;
    constexpr atomic() = default;
    T                            value() const;
    T                            value(std::size_t ndx) const;
    void                         set_value(const T& data);
    void                         set_value(const T& data, std::size_t ndx);
    constexpr static std::size_t size();
    constexpr static std::size_t el_max_size() { return sizeof(T); }
    int16_t                      bind_par(std::int32_t a_stmt_handle, std::int16_t a_ndx)
    {
      auto rc = SQLBindParameter(a_stmt_handle,
                                 a_ndx,
                                 SQL_PARAM_INPUT,
                                 (c_type(DB_TYPE)),
                                 (DB_TYPE),
                                 el_max_size(),
                                 dec_,
                                 static_cast<void*>(data_.data()),
                                 el_max_size(),
                                 static_cast<int*>(len_.data()));
      return rc;
    }
    int16_t bind_col(std::int32_t a_stmt_handle, std::int16_t a_ndx)
    {
      auto rc = SQLBindCol(a_stmt_handle,
                           a_ndx,
                           //                           SQL_C_DEFAULT,
                           c_type(DB_TYPE),
                           static_cast<void*>(data_.data()),
                           this->el_max_size(),
                           static_cast<int*>(len_.data()));
      return rc;
    }
    std::string dump(cstr_t a_msg, uint a_ndx, uint offs) const;
    std::string dump_all(cstr_t a_msg = "", std::size_t max_el = size(), uint offs = 0) const;
  private:
    t_vec                          data_{};            //!< value buffer
    l_vec                          len_{};             //!< value length
    constexpr static const int16_t type_  = DB_TYPE;   //!< parameter type
    constexpr static const int32_t width_ = sizeof(T); //!< parameter width
    constexpr static const int16_t dec_   = DEC;       //!< decimal places
  };
  /*....bstring.............................................................................*/
  /**
   * @brief block of memory with at most <n> bytes in lenght
   *
   * @tparam T
   * @tparam arr_size
   */
  template <std::size_t blk_size, std::size_t arr_size = 1, typename EL = std::byte>
  class bstring
  {
  public:
    using T     = std::array<EL, blk_size>;
    using T_ref = std::span<const EL>;
    using t_vec = std::array<T, arr_size>;
    using l_vec = std::array<int32_t, arr_size>;
    T_ref value() const;
    T_ref value(std::size_t ndx) const;

    void set_value(const T_ref data);
    void set_value(const T_ref data, std::size_t ndx);

    consteval std::size_t size() const;
    consteval std::size_t el_max_size() const;
    std::string           dump(cstr_t a_msg, uint a_ndx, uint offs) const;
    static std::string    serialize(T_ref blk);
    std::string dump_all(cstr_t a_msg = "", std::size_t max_el = arr_size, uint offs = 0) const;
  private:
    t_vec data_{};
    l_vec len_{}; //!< value length
  public:
  };
  /*........................................................................................*/
  /**
   * @brief
   *
   * @tparam item_size
   * @tparam arr_size
   * @tparam T         type of character TODO works only for char
   */
  template <std::size_t item_size, std::size_t arr_size = 1, typename T = char>
  class string
  {
  public:
    using t_vec = std::array<std::array<T, item_size + 1>, arr_size>;
    using l_vec = std::array<int32_t, arr_size>;
    cstr_t                value() const;
    cstr_t                value(std::size_t ndx) const;
    void                  set_value(cstr_t data);
    void                  set_value(cstr_t data, std::size_t ndx);
    consteval std::size_t size();
    std::string           dump(cstr_t a_msg, uint a_ndx, uint offs) const;
    std::string dump_all(cstr_t a_msg = "", std::size_t max_el = arr_size, uint offs = 0) const;
  private:
    t_vec data_{};
    l_vec len_{}; //!< value length
  public:
  };
  /*....inline methods .......................................................................*/

  template <std::size_t item_size, std::size_t arr_size, typename T>
  inline cstr_t string<item_size, arr_size, T>::value() const
  {
    return value(0);
  }

  template <std::size_t item_size, std::size_t arr_size, typename T>
  inline cstr_t string<item_size, arr_size, T>::value(std::size_t ndx) const
  {
    auto ref = this->data_.at(ndx).data();
    return cstr_t(ref, std::strlen(ref));
  }

  template <std::size_t item_size, std::size_t arr_size, typename T>
  inline void string<item_size, arr_size, T>::set_value(cstr_t data)
  {
    set_value(data, 0);
  }

  template <std::size_t item_size, std::size_t arr_size, typename T>
  inline void string<item_size, arr_size, T>::set_value(cstr_t data, std::size_t ndx)
  {
    //    const std::size_t max_len = data_.at(0).size() - 1; // trailing \0
    auto src = data.data();
    auto dst = data_.at(ndx).data();
    if (src != dst)
    {
      // too long inputs are trimmed. This is on purpose.
      const std::size_t max_len = std::min(item_size, data.size());
      std::memcpy(dst, src, max_len * sizeof(T));
      dst[max_len] = 0;
    }
  }

  template <std::size_t item_size, std::size_t arr_size, typename T>
  consteval inline std::size_t string<item_size, arr_size, T>::size()
  {
    return arr_size;
  }

  template <std::size_t item_size, std::size_t arr_size, typename T>
  inline std::string string<item_size, arr_size, T>::dump(cstr_t a_msg, uint a_ndx, uint offs) const
  {
    std::stringstream s;
    s << std::string(offs, ' ') << a_msg << value(a_ndx);
    return s.str();
  }

  template <std::size_t item_size, std::size_t arr_size, typename T>
  inline std::string string<item_size, arr_size, T>::dump_all(cstr_t      a_msg,
                                                              std::size_t max_el,
                                                              uint        offs) const
  {
    std::string s;
    for (auto cnt = 0UL; cnt < std::min(data_.size(), max_el); ++cnt)
      s += std::string("'") + std::string(value(cnt)) + "' ";
    s = std::string(offs, ' ') + std::string(a_msg) + s;
    return s;
  }

  template <typename T, int DB_TYPE, int DEC, std::size_t arr_size>
  inline T atomic<T, DB_TYPE, DEC, arr_size>::value() const
  {
    return data_[0];
  }

  template <typename T, int DB_TYPE, int DEC, std::size_t arr_size>
  inline T atomic<T, DB_TYPE, DEC, arr_size>::value(std::size_t ndx) const
  {
    return this->data_.at(ndx);
  }

  template <typename T, int DB_TYPE, int DEC, std::size_t arr_size>
  inline void atomic<T, DB_TYPE, DEC, arr_size>::set_value(const T& data)
  {
    set_value(data, 0);
  }

  template <typename T, int DB_TYPE, int DEC, std::size_t arr_size>
  inline void atomic<T, DB_TYPE, DEC, arr_size>::set_value(const T& data, std::size_t ndx)
  {
    this->data_.at(ndx) = data;
  }

  template <typename T, int DB_TYPE, int DEC, std::size_t arr_size>
  constexpr inline std::size_t atomic<T, DB_TYPE, DEC, arr_size>::size()
  {
    return arr_size;
  }

  template <typename T, int DB_TYPE, int DEC, std::size_t arr_size>
  inline std::string atomic<T, DB_TYPE, DEC, arr_size>::dump(cstr_t a_msg,
                                                             uint   a_ndx,
                                                             uint   offs) const
  {
    std::stringstream s;
    s << std::string(offs, ' ') << a_msg << value(a_ndx);
    return s.str();
  }

  template <typename T>
  inline std::string serialize(const T& val)
  {
    std::stringstream ss;
    ss << val;
    return ss.str();
  }
  template <>
  inline std::string serialize<SQL_TIMESTAMP_STRUCT>(const SQL_TIMESTAMP_STRUCT& val)
  {
    std::string s;
    // clang-format off
    s += 
    std::to_string(val.year) + "-" + std::to_string(val.month) + "-" + std::to_string(val.day) + " " +
    std::to_string(val.hour) + ":" +
    std::to_string(val.minute) + ":" +
    std::to_string(val.second) + "." +
    std::to_string(val.fraction) 
    ;
    // clang-format on
    return s;
  }
  template <>
  inline std::string serialize<SQL_DATE_STRUCT>(const SQL_DATE_STRUCT& val)
  {
    std::string s;
    // clang-format off
    s += 
    std::to_string(val.year) + "-" + std::to_string(val.month) + "-" + std::to_string(val.day)
    ;
    // clang-format on
    return s;
  }
  template <>
  inline std::string serialize<SQL_TIME_STRUCT>(const SQL_TIME_STRUCT& val)
  {
    std::string s;
    // clang-format off
    s += 
    std::to_string(val.hour) + ":" +
    std::to_string(val.minute) + ":" +
    std::to_string(val.second)
    ;
    // clang-format on
    return s;
  }
  template <typename T, int DB_TYPE, int DEC, std::size_t arr_size>
  inline std::string atomic<T, DB_TYPE, DEC, arr_size>::dump_all(cstr_t      a_msg,
                                                                 std::size_t max_el,
                                                                 uint        offs) const
  {
    std::string s;
    for (auto cnt = 0UL; cnt < std::min(data_.size(), max_el); ++cnt)
    {
      s += serialize<T>(data_[cnt]);
    }
    s = std::string(offs, ' ') + std::string(a_msg) + s;
    return s;
  }
  template <std::size_t blk_size, std::size_t arr_size, typename EL>
  inline auto bstring<blk_size, arr_size, EL>::value() const -> T_ref
  {
    return value(0);
  }

  template <std::size_t blk_size, std::size_t arr_size, typename EL>
  inline auto bstring<blk_size, arr_size, EL>::value(std::size_t ndx) const -> T_ref
  {
    return T_ref(data_.at(ndx).data(), len_.at(ndx));
  }

  template <std::size_t blk_size, std::size_t arr_size, typename EL>
  inline void bstring<blk_size, arr_size, EL>::set_value(const T_ref data)
  {
    set_value(data, 0);
  }

  template <std::size_t blk_size, std::size_t arr_size, typename EL>
  inline void bstring<blk_size, arr_size, EL>::set_value(const T_ref data, std::size_t ndx)
  {
    auto src = data.data();
    auto dst = data_.at(ndx).data();
    if (src != dst)
    {
      auto len = std::min(data.size(), blk_size);
      std::memcpy(dst, src, len);
    }
  }

  template <std::size_t blk_size, std::size_t arr_size, typename EL>
  consteval std::size_t bstring<blk_size, arr_size, EL>::size() const
  {
    return arr_size;
  }

  template <std::size_t blk_size, std::size_t arr_size, typename EL>
  consteval std::size_t bstring<blk_size, arr_size, EL>::el_max_size() const
  {
    return (blk_size);
  }

  template <std::size_t blk_size, std::size_t arr_size, typename EL>
  inline std::string bstring<blk_size, arr_size, EL>::dump(cstr_t a_msg,
                                                           uint   a_ndx,
                                                           uint   offs) const
  {
    std::stringstream s;
    s << std::string(offs, ' ') << a_msg << value(a_ndx);
    return s.str();
  }

  template <std::size_t blk_size, std::size_t arr_size, typename EL>
  inline std::string bstring<blk_size, arr_size, EL>::serialize(T_ref blk)
  {
    std::stringstream s;
    for (const auto& byte : blk) s << std::hex << std::to_integer<uint8_t>(byte) << " ";
    return s.str();
  }

  template <std::size_t blk_size, std::size_t arr_size, typename EL>
  inline std::string bstring<blk_size, arr_size, EL>::dump_all(cstr_t      a_msg,
                                                               std::size_t max_el,
                                                               uint        offs) const
  {
    std::string s;
    s.reserve(offs + a_msg.size() + arr_size * blk_size * (sizeof(EL) * 2 + 1));
    s = std::string(offs, ' ') + std::string(a_msg);
    for (auto cnt = 0UL; cnt < std::min(data_.size(), max_el); ++cnt)
    {
      // const bstring::T_ref item(data_[cnt].data(), len_[cnt]);
      s += serialize(value(cnt));
    }
    return s;
  }

};     // namespace db
#endif // DBGEN3_TEMPL_HPP
