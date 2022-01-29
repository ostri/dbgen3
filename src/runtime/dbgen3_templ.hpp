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

namespace db
{
  using cstr_t = std::string_view;
  using str_t  = std::string;

  //  using a5 = std::array<char, 5>;
  namespace
  {
    struct type_dscr
    {
      int c_type_  = 0; //!< C type
      int db_type_ = 0; //!< equivalent ODBC type
    };
    // clang-format off
    const std::map<int, type_dscr> dscr_ = 
    {
      { 5, {SQL_C_SHORT, SQL_SMALLINT}},    //!<smallint
      { 4, {SQL_C_LONG, SQL_INTEGER}},     //!<int
      {-5, {SQL_C_SBIGINT, SQL_BIGINT}},  //!<signed bigint
    };
    // clang-format on
    static inline const type_dscr& dscr(int a_type)
    {
      if (dscr_.contains(a_type)) return dscr_.at(a_type);
      throw std::runtime_error("undefined type:"+std::to_string(a_type));
    }
  }; // namespace
  template <typename T, std::size_t arr_size = 1>
  class atomic
  {
  public:
    using t_vec = std::array<T, arr_size>;
    atomic(int16_t type, /*int32_t width,*/ int16_t dec)
    : type_(type)
    , width_(sizeof(T))
    , dec_(dec)
    { }
    T                     value() const;
    T                     value(std::size_t ndx) const;
    void                  set_value(const T& data);
    void                  set_value(const T& data, std::size_t ndx);
    consteval std::size_t size();
    constexpr std::size_t el_max_size() const { return sizeof(T); }
    int16_t               bind_par(std::int32_t a_stmt_handle, std::int16_t a_ndx)
    {
      int32_t len = 0;
      auto    rc  = SQLBindParameter(a_stmt_handle,
                                 a_ndx,
                                 SQL_PARAM_INPUT,
                                 dscr(type_).c_type_,
                                 dscr(type_).db_type_,
                                 this->el_max_size(),
                                 dec_,
                                 static_cast<void*>(data_.data()),
                                 this->el_max_size(),
                                 &len);
      return rc;
    }
    int16_t bind_col(std::int32_t a_stmt_handle, std::int16_t a_ndx)
    {
      int32_t len = 0;
      auto    rc  = SQLBindCol(a_stmt_handle,
                           a_ndx,
                           SQL_C_DEFAULT,
                           static_cast<void*>(data_.data()),
                           this->el_max_size(),
                           &len);
      return rc;
    }
    std::string dump(cstr_t a_msg, uint a_ndx, uint offs) const
    {
      std::stringstream s;
      s << std::string(offs, ' ') << a_msg << value(a_ndx);
      return s.str();
    }
    std::string dump_all(cstr_t a_msg, std::size_t max_el, uint offs = 0) const
    {
      std::string s;
      for (auto cnt = 0UL; cnt < std::min(data_.size(), max_el); ++cnt)
        s += std::to_string(value(cnt)) + " ";
      s = std::string(offs, ' ') + std::string(a_msg) + s;
      return s;
    }
  private:
    t_vec   data_{}; //!< value buffer
    int16_t type_;   //!< parameter type
    int32_t width_=sizeof(T);  //!< parameter width
    int16_t dec_;    //!< decimal places
  public:
  };
  /**
   * @brief block of memory with at most <n> bytes in lenght
   *
   * @tparam T
   * @tparam arr_size
   */
  template <std::size_t blk_size, std::size_t arr_size = 1, typename EL = char>
  class mem_blk
  {
  public:
    using T     = std::array<EL, blk_size>;
    using T_ref = std::span<const EL>;
    using t_vec = std::array<T, arr_size>;
    T_ref value() const { return value(0); }
    T_ref value(std::size_t ndx) const { return T_ref(this->data_.at(ndx).data(), el_max_size()); }

    void set_value(const T_ref data) { set_value(data, 0); }
    void set_value(const T_ref data, std::size_t ndx)
    {
      auto len = std::min(data.size(), el_max_size());
      std::copy_n(data.begin(), len, data_.at(ndx).begin());
    }

    std::size_t size() const { return data_.size(); }
    std::size_t el_max_size() const { return data_[0].size(); }
  private:
    t_vec data_{};
  public:
  };

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
    cstr_t      value() const;
    cstr_t      value(std::size_t ndx) const;
    void        set_value(cstr_t data);
    void        set_value(cstr_t data, std::size_t ndx);
    std::size_t size();
  private:
    t_vec data_{};
  public:
  };

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
    const std::size_t max_len = data_.at(0).size() - 1; // trailing \0
    auto              src     = data.data();
    auto              dst     = data_.at(ndx).data();
    if (src != dst)
    {
      // too long inputs are trimmed. This is on purpose.
      std::strncpy(dst, src, max_len);
      dst[max_len] = 0;
    }
  }

  template <std::size_t item_size, std::size_t arr_size, typename T>
  inline std::size_t string<item_size, arr_size, T>::size()
  {
    return data_.size();
  }

  template <typename T, std::size_t arr_size>
  inline T atomic<T, arr_size>::value() const
  {
    return data_[0];
  }

  template <typename T, std::size_t arr_size>
  inline T atomic<T, arr_size>::value(std::size_t ndx) const
  {
    return this->data_.at(ndx);
  }

  template <typename T, std::size_t arr_size>
  inline void atomic<T, arr_size>::set_value(const T& data)
  {
    set_value(data, 0);
  }

  template <typename T, std::size_t arr_size>
  inline void atomic<T, arr_size>::set_value(const T& data, std::size_t ndx)
  {
    this->data_.at(ndx) = data;
  }

  template <typename T, std::size_t arr_size>
  inline consteval std::size_t atomic<T, arr_size>::size()
  {
    return data_.size();
  }
};     // namespace db
#endif // DBGEN3_TEMPL_HPP
