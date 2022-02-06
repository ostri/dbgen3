#ifndef DBGEN3_TEMPL_HPP
#define DBGEN3_TEMPL_HPP

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <map>
#include <span>
#include <sqlcli1.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

#include "odbc_db.hpp"
#include "serialize_templ.hpp"
namespace db
{
  using cstr_t  = std::string_view;
  using str_t   = std::string;
  using bstr_t  = std::span<uint8_t>;
  using cbstr_t = std::span<const uint8_t>;

  /*....root class for all attributes ........................................................*/
  template <std::size_t arr_size>
  class attr_root_root
  {
  public:
    using l_vec        = std::array<int32_t, arr_size>;
    virtual str_t dump_all( //
      cstr_t      a_msg,
      std::size_t max_el,
      uint        offs) const = 0;

    constexpr virtual std::size_t size() const = 0;
    virtual l_vec&                len()        = 0;
  };
  /**
   * @brief root class for all attributes within the buffers
   *
   */
  template <typename T, int16_t DB_TYPE, std::size_t arr_size, std::size_t DEC>
  class attr_root : public attr_root_root<arr_size>
  {
  public:
    using t_vec = std::array<T, arr_size>;
    using l_vec = std::array<int32_t, arr_size>;
    // using bstr_t                    = std::span<uint8_t>;
    using cstr_t                    = std::string_view;
    attr_root()                     = default;
    virtual ~attr_root()            = default;
    attr_root(const attr_root&)     = default;
    attr_root(attr_root&&) noexcept = default;
    attr_root&            operator=(const attr_root&) = default;
    attr_root&            operator=(attr_root&&) noexcept = default;
    constexpr std::size_t size() const { return static_cast<std::size_t>(arr_size); }
    l_vec&                len() { return len_; }
    const l_vec&          len() const { return len_; }
    t_vec&                data() { return data_; }
    const t_vec&          data() const { return data_; }
    constexpr auto        width() const { return width_; }
    constexpr auto        db_type() const { return db_type_; }
    /// size of specific element
    std::size_t el_size(std::size_t ndx) const { return static_cast<std::size_t>(len_.at(ndx)); }
    /// value getters
    bstr_t v_bstr(size_t ndx)
    { // be careful this can be altered from the outside
      if constexpr (is_bstring(db_type_)) { return bstr_t{data_.at(ndx).data(), el_size(ndx)}; }
      return {};
    };
    cbstr_t v_bstr(size_t ndx) const
    {
      if constexpr (is_bstring(db_type_))
      { // must change uint8_t -> const uint8_t so that it can not be altered from outside
        // NOLINTNEXTLINE clang-tidy(cppcoreguidelines-pro-type-reinterpret-cast)
        cbstr_t r(reinterpret_cast<const uint8_t*>(&data_.at(ndx).at(0)), el_size(ndx));
        return r;
      }
      return {};
    };
    cstr_t v_cstr(size_t ndx)
    {
      if constexpr (is_string(db_type_)) { return cstr_t{data_.at(ndx).data(), el_size(ndx)}; }
      return {};
    };
    cstr_t v_cstr(size_t ndx) const
    {
      if constexpr (is_string(db_type_)) { return cstr_t{data_.at(ndx).data(), el_size(ndx)}; }
      else return {};
    };
    T v_T(size_t ndx)
    {
      if constexpr (! std::is_class<T>::value) { return data_.at(ndx); }
      else return {};
    };
    T v_T(size_t ndx) const
    {
      if constexpr (! std::is_class<T>::value) { return data_.at(ndx); }
      else return {};
    };
    void s_T(const T& v, std::size_t ndx)
    {
      data_.at(ndx) = v;
      len_.at(ndx)  = sizeof(T);
    }
    void s_cstr(cstr_t v, std::size_t ndx)
    { /// code for s_cstr and s_bstr is a duplicate on purpose
      /// unless I impose is_string and is_bstring I get random errors
      if constexpr (is_string(db_type_))
      {
        // too long values are removed by purpose
        auto        max_len = std::min(sizeof(T), v.size());
        const auto* src     = v.data();
        auto        dst     = data_.at(ndx).data();
        if (src != dst)
        {
          std::memcpy(dst, src, max_len);
          len_.at(ndx) = max_len;
        }
      }
    }
    void s_bstr(cbstr_t v, std::size_t ndx)
    {
      if constexpr (is_bstring(db_type_))
      {
        // too long values are removed by purpose
        auto        max_len = std::min(sizeof(T), v.size());
        const auto* src     = v.data();
        auto        dst     = data_.at(ndx).data();
        if (src != dst)
        {
          std::memcpy(dst, src, max_len);
          len_.at(ndx) = max_len;
        }
      }
    }
    str_t dump_all(cstr_t a_msg, std::size_t max_el, uint offs) const
    {
      std::string s;
      if (max_el == 0) max_el = arr_size;
      s = std::string(offs, ' ') + std::string(a_msg) + "[";
      for (auto cnt = 0UL; cnt < std::min(arr_size, max_el); ++cnt)
      {
        auto grp = attr_type(db_type_);
        switch (grp)
        {
        case db::ATTR_TYPE::atomic:
          /*if constexpr (is_atomic(db_type_))*/ s += serialize<T>(v_T(cnt));
          break;
        case db::ATTR_TYPE::string:
          /*if constexpr (is_string(db_type_))*/ s += serialize<cstr_t>(v_cstr(cnt));
          break;
        case db::ATTR_TYPE::bstring:
          /*if constexpr (is_bstring(db_type_))*/ s += serialize<cbstr_t>(v_bstr(cnt));
          break;
        case db::ATTR_TYPE::unknown:
          throw std::runtime_error("switch error: " + std::string(__FILE__) + " " +
                                   std::to_string(__LINE__));
        }
      }
      s += "]";
      return s;
    }
    int16_t bind_par(std::int32_t a_stmt_handle, std::int16_t a_ndx)
    {
      auto rc = SQLBindParameter(a_stmt_handle,
                                 a_ndx,
                                 SQL_PARAM_INPUT,
                                 c_type(db_type_),
                                 db_type_,
                                 width(),
                                 dec_,
                                 static_cast<void*>(data_.data()),
                                 width(),
                                 static_cast<int*>(len_.data()));
      return rc;
    }
  protected:
  private:
    l_vec                          len_{};               //!< value length
    t_vec                          data_{};              //!< values
    constinit static const int32_t width_   = sizeof(T); //!< parameter width
    constinit static const int16_t db_type_ = DB_TYPE;   //!< parameter type
    constinit static const int16_t dec_     = DEC;       //!< decimal places
  };
  /*....root class for all buffers  ........................................................*/
  /**
   * @brief root class for all buffers
   *
   */
  class buffer_root
  {
  public:
    buffer_root()                   = default;
    virtual ~buffer_root()          = default;
    buffer_root(const buffer_root&) = default;
    buffer_root(buffer_root&&)      = default;
    buffer_root&  operator=(const buffer_root&) = default;
    buffer_root&  operator=(buffer_root&&) = default;
    virtual str_t dump()                   = 0;
  protected:
  private:
  };
  /*....atomic...............................................................................*/
  template <typename T, int16_t DB_TYPE, int DEC, std::size_t arr_size>
  class atomic : public attr_root<T, DB_TYPE, arr_size, DEC>
  {
  public:
    using t_vec        = std::array<T, arr_size>;
    using l_vec        = std::array<int32_t, arr_size>;
    using PP           = attr_root<T, DB_TYPE, arr_size, DEC>;
    constexpr atomic() = default;
    T       value() const { return value(0); }
    T       value(std::size_t ndx) const { return PP::data().at(ndx); }
    void    set_value(const T& data) { set_value(data, 0); }
    void    set_value(const T& data, std::size_t ndx) { PP::data().at(ndx) = data; }
    int16_t bind_par(std::int32_t a_stmt_handle, std::int16_t a_ndx)
    {
      auto rc = SQLBindParameter(a_stmt_handle,
                                 a_ndx,
                                 SQL_PARAM_INPUT,
                                 (c_type(DB_TYPE)),
                                 (DB_TYPE),
                                 PP::width(),
                                 dec_,
                                 static_cast<void*>(PP::data().data()),
                                 PP::width(),
                                 static_cast<int*>(PP::len().data()));
      return rc;
    }
    int16_t bind_col(std::int32_t a_stmt_handle, std::int16_t a_ndx)
    {
      auto rc = SQLBindCol(a_stmt_handle,
                           a_ndx,
                           //                           SQL_C_DEFAULT,
                           c_type(DB_TYPE),
                           static_cast<void*>(PP::data().data()),
                           PP::width(),
                           static_cast<int*>(PP::len().data()));
      return rc;
    }
    std::string dump_all(cstr_t a_msg, std::size_t max_el, uint offs) const override
    {
      return PP::dump_all(a_msg, max_el, offs);
    }
  private:
    constinit static const int16_t dec_ = DEC; //!< decimal places
  };
  /*....bstring.............................................................................*/
  /**
   * @brief block of memory with at most <n> bytes in lenght
   *
   * @tparam T
   * @tparam arr_size
   */
  template <typename T, int16_t DB_TYPE, std::size_t arr_size, typename EL, std::size_t DEC>
  class bstring : public attr_root<T, DB_TYPE, arr_size, DEC>
  {
  public:
    using PP            = attr_root<T, DB_TYPE, arr_size, DEC>;
    constexpr bstring() = default;
    bstr_t  value() { return value(0); }
    cbstr_t value() const { return value(0); }
    bstr_t  value(std::size_t ndx) { return PP::v_bstr(ndx); }
    cbstr_t value(std::size_t ndx) const
    {
      assert(typeid(EL).hash_code() == typeid(uint8_t).hash_code());
      return PP::v_bstr(ndx);
    }
    void        set_value(cbstr_t data) { set_value(data, 0); }
    void        set_value(cbstr_t data, std::size_t ndx) { PP::s_bstr(data, ndx); }
    std::string dump_all(cstr_t a_msg, std::size_t max_el, uint offs) const override
    {
      return PP::dump_all(a_msg, max_el, offs);
    }
  private:
  public:
  };
  /*........................................................................................*/
  /**
   * @brief string
   *
   * @tparam item_size
   * @tparam arr_size
   * @tparam T         type of character TODO works only for char
   *
   *     FIXME it is assumed that the string is made of one byte characters
   *
   */
  template <typename T, int16_t DB_TYPE, std::size_t arr_size, typename EL, std::size_t DEC>
  class string : public attr_root<T, DB_TYPE, arr_size, DEC>
  {
  public:
    using PP           = attr_root<T, DB_TYPE, arr_size, DEC>;
    constexpr string() = default;
    cstr_t      value() const { return value(0); }
    cstr_t      value(std::size_t ndx) const { return PP::v_cstr(ndx); }
    void        set_value(cstr_t data) { set_value(data, 0); }
    void        set_value(cstr_t data, std::size_t ndx) { PP::s_cstr(data, ndx); }
    std::string dump_all(cstr_t a_msg, std::size_t max_el, uint offs) const override
    {
      return PP::dump_all(a_msg, max_el, offs);
    }
  private:
  public:
  };
  /*....inline methods .......................................................................*/

};     // namespace db
#endif // DBGEN3_TEMPL_HPP
