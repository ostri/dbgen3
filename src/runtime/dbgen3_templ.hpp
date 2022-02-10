#ifndef DBGEN3_TEMPL_HPP
#define DBGEN3_TEMPL_HPP

#include <array>
#include <cassert>
#include <cstddef>
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
#include <vector>

#include "odbc_db.hpp"
#include "connection.hpp"
#include "serialize_templ.hpp"
#include "statement.hpp"
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
    attr_root_root()                          = default;
    virtual ~attr_root_root()                 = default;
    attr_root_root(const attr_root_root&)     = default;
    attr_root_root(attr_root_root&&) noexcept = default;
    attr_root_root& operator=(const attr_root_root&) = default;
    attr_root_root& operator=(attr_root_root&&) noexcept = default;

    using l_vec = std::array<int32_t, arr_size>;
    virtual str_t                 dump_all(cstr_t a_msg, std::size_t max_el, uint offs) const = 0;
    virtual int16_t               bind_par(std::int32_t a_stmt_handle, std::int16_t a_ndx)    = 0;
    virtual int16_t               bind_col(std::int32_t a_stmt_handle, std::int16_t a_ndx)    = 0;
    constexpr virtual std::size_t size() const                                                = 0;
    virtual l_vec&                len()                                                       = 0;
    virtual const l_vec&          len() const                                                 = 0;
    // virtual constexpr int         width() const                                               =
    // 0; virtual constexpr int         dec() const = 0; virtual constexpr int         db_type()
    // const                                             = 0;
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
    constexpr std::size_t size() const final { return static_cast<std::size_t>(arr_size); }
    l_vec&                len() final { return len_; }
    const l_vec&          len() const final { return len_; }
    t_vec&                data() { return data_; }
    const t_vec&          data() const { return data_; }
    constexpr int32_t     width() { return width_; }
    constexpr int16_t     dec() const { return dec_; }
    constexpr int16_t     db_type() const { return db_type_; }
    /// size of specific element
    size_t el_size(std::size_t ndx) const { return static_cast<size_t>(len_.at(ndx)); }
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
      if constexpr (is_atomic(db_type_)) { return data_.at(ndx); }
      else return {};
    };
    T v_T(size_t ndx) const
    {
      if constexpr (is_atomic(db_type_)) { return data_.at(ndx); }
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
    str_t dump_all(cstr_t a_msg, std::size_t max_el, uint offs) const override
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
    int16_t bind_par(std::int32_t a_stmt_handle, std::int16_t a_ndx) override
    {
      std::cerr << ct_name(db_type_) << "--" << dbt_name(db_type_) << "-- " << width() << "--" << dec_ << std::endl;
      auto rc = SQLBindParameter(a_stmt_handle,
                                 a_ndx,
                                 SQL_PARAM_INPUT,
                                 c_type(db_type_),
                                 db_type_,
                                 width()-1,
                                 dec_,
                                 static_cast<void*>(data_.data()),
                                 width(),
                                 static_cast<int*>(len_.data()));
      return rc;
    }
    int16_t bind_col(std::int32_t a_stmt_handle, std::int16_t a_ndx) override
    {
      auto rc = SQLBindCol(a_stmt_handle,
                           a_ndx,
                           c_type(DB_TYPE),
                           static_cast<void*>(data_.data()),
                           width_,
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
  /**
   * @brief buffer root class as seen from programmer (no dimennsion only pure methods)
   *
   */
  class buffer_root_root
  {
  public:
    using s_vec_a                                      = std::span<SQLUSMALLINT>;
    virtual str_t   dump(cstr_t a_msg) const           = 0;
    virtual int16_t bind(std::int32_t a_stmt_handle)   = 0;
    virtual int16_t rebind(std::int32_t a_stmt_handle) = 0;
    virtual size_t  size() const                       = 0;
    virtual size_t  occupied() const                   = 0;
    virtual void    set_occupied(size_t v)             = 0;
    virtual size_t* occupied_addr() = 0; // so that cli can write how many records are read
    virtual constexpr BUF_TYPE bt() = 0;
    // // parameter only
    virtual void*       s_vec()           = 0;
    virtual const void* s_vec() const     = 0;
    virtual size_t&     procesed()        = 0;
    virtual size_t      processed() const = 0;
  private:
  };
  /*....root class for all buffers  ........................................................*/
  /**
   * @brief root class for all buffers
   *
   * arr_size number of values stored in each attribute thaz is part of the buffer
   */
  template <BUF_TYPE BT, std::size_t arr_size>
  class buffer_root : public buffer_root_root
  {
  public:
    using attr_vec_t                    = std::vector<attr_root_root<arr_size>*>;
    using s_vec_t                       = std::array<SQLUSMALLINT, arr_size>;
    using s_vec_a                       = std::span<SQLUSMALLINT>;
    buffer_root()                       = default;
    virtual ~buffer_root()              = default;
    buffer_root(const buffer_root&)     = default;
    buffer_root(buffer_root&&) noexcept = default;
    buffer_root& operator=(const buffer_root&) = default;
    buffer_root& operator=(buffer_root&&) noexcept = default;
    size_t       size() const override { return arr_size; }
    void*        s_vec() override { return s_vec_.data(); }
    const void*  s_vec() const override { return s_vec_.data(); }
    size_t&      procesed() override { return processed_; }
    size_t       processed() const override { return processed_; }

    str_t dump(cstr_t a_msg) const override
    {
      str_t r;
      r += str_t(a_msg) + "\n";
      r += "# attr:   " + std::to_string(attr_vec_.size()) + "\n";
      ;
      r += "# lines:  " + std::to_string(size()) + "\n";
      r += "#occupied:" + std::to_string(occupied_) + "\n";
      if (bt_ == BUF_TYPE::par)
      {
        r += "#processed:" + std::to_string(processed_) + "\n";
        std::span tmp(s_vec_.data(), processed_);
        r += "status vec: ";
        for (const auto& el : tmp) r += std::to_string(el) + " ";
      }
      r += "\n";
      r += "buffer values:\n";
      for (const auto& el : attr_vec_)
        r += el->dump_all(std::to_string((&el - &attr_vec_[0])), occupied(), 0) + "\n";
      return r;
    }
    int16_t rebind(std::int32_t a_stmt_handle) override
    {
      if constexpr (bt_ == BUF_TYPE::par) return bind_par(a_stmt_handle);
      else return bind_col(a_stmt_handle);
    }
    int16_t bind(std::int32_t a_stmt_handle) override
    {
      if (bind_) return SQL_SUCCESS;
      return rebind(a_stmt_handle);
    }
    //    bool   is_bind() const override { return bind_; }
    //    size_t size() const override { return arr_size; }
    virtual attr_vec_t& attr_vec() { return attr_vec_; }
    size_t              occupied() const override { return occupied_; }
    void                set_occupied(size_t v) override { occupied_ = v; }
    size_t*             occupied_addr() override { return &occupied_; }
    constexpr BUF_TYPE  bt() override { return buffer_root::bt_; }
  private:
    int16_t bind_par(std::int32_t a_stmt_handle)
    {
      if constexpr (bt_ == BUF_TYPE::par)
      {
        int16_t rc  = SQL_SUCCESS;
        auto    ndx = 0;
        for (const auto& el : attr_vec_)
        {
          rc = el->bind_par(a_stmt_handle, ++ndx);
          if (rc != SQL_SUCCESS) return rc;
        }
        bind_ = true;
        return rc;
      }
    }
    int16_t bind_col(std::int32_t a_stmt_handle)
    {
      if constexpr (bt_ == BUF_TYPE::res)
      {
        int16_t rc  = SQL_SUCCESS;
        auto    ndx = 0;
        for (const auto& el : attr_vec_)
        {
          rc = el->bind_col(a_stmt_handle, ++ndx);
          if (rc != SQL_SUCCESS) return rc;
        }
        bind_ = true;
        return rc;
      }
    }
    /*.......................................................................................*/
    constexpr static std::size_t arr_size_ = arr_size; //!< number of values in each attribute
    //  TODO(ostri): sometimes arr_vec_t can be sts::array
    constexpr static BUF_TYPE bt_ = BT;       //!< buffer type
    size_t                    occupied_{};    //!< how many records is occupied [0..size())
    attr_vec_t                attr_vec_ = {}; //!< attributes of the buffer
    bool                      bind_{};        //!< are the parameters bound to the statement?
    s_vec_t                   s_vec_{};       //!< vector of statuses
    size_t                    processed_{};   //!< number of parameters processed
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
    int16_t bind_par(std::int32_t a_stmt_handle, std::int16_t a_ndx) override
    {
      return PP::bind_par(a_stmt_handle, a_ndx);
    }
    int16_t bind_col(std::int32_t a_stmt_handle, std::int16_t a_ndx) override
    {
      return PP::bind_col(a_stmt_handle, a_ndx);
    }
    std::string dump_all(cstr_t a_msg, std::size_t max_el, uint offs) const override
    {
      return PP::dump_all(a_msg, max_el, offs);
    }
  private:
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
    void    set_value(cbstr_t data) { set_value(data, 0); }
    void    set_value(cbstr_t data, std::size_t ndx) { PP::s_bstr(data, ndx); }
    int16_t bind_par(std::int32_t a_stmt_handle, std::int16_t a_ndx) override
    {
      return PP::bind_par(a_stmt_handle, a_ndx);
    }
    int16_t bind_col(std::int32_t a_stmt_handle, std::int16_t a_ndx) override
    {
      return PP::bind_col(a_stmt_handle, a_ndx);
    }
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
    cstr_t  value() const { return value(0); }
    cstr_t  value(std::size_t ndx) const { return PP::v_cstr(ndx); }
    void    set_value(cstr_t data) { set_value(data, 0); }
    void    set_value(cstr_t data, std::size_t ndx) { PP::s_cstr(data, ndx); }
    int16_t bind_par(std::int32_t a_stmt_handle, std::int16_t a_ndx) override
    {
      return PP::bind_par(a_stmt_handle, a_ndx);
    }
    int16_t bind_col(std::int32_t a_stmt_handle, std::int16_t a_ndx) override
    {
      return PP::bind_col(a_stmt_handle, a_ndx);
    }
    std::string dump_all(cstr_t a_msg, std::size_t max_el, uint offs) const override
    {
      return PP::dump_all(a_msg, max_el, offs);
    }
  private:
  public:
  };
  class utl
  {
  public:
    using brr_t = buffer_root_root;
    utl()       = delete;
    utl(db::connection* c, cstr_t sql)
    : utl(c, sql, nullptr, nullptr) // no parameters or results
    { }
    utl(db::connection* c, cstr_t sql, brr_t* ptr) // parameter or result
    : utl(c,
          sql,
          chk2(ptr, BUF_TYPE::par),
          chk(ptr, "You must provide proper buffer object generated by dbgen3."))
    { }
    utl(db::connection* c, cstr_t sql, brr_t* p_buf, brr_t* r_buf) // all provided
    : s_(c, sql)
    , par_buf_(utl::chk(p_buf, "Wrong buf type. First is PARAMETER."))
    , res_buf_(utl::chk(r_buf, "Wrong buf type. Second is RESULT."))
    { }
    static brr_t* chk(brr_t* ptr, cstr_t a_msg)
    {
      if (ptr != nullptr)
      {
        if (ptr->bt() == BUF_TYPE::par) return ptr;
        throw std::runtime_error(str_t(a_msg));
      }
      return nullptr;
    }
    static brr_t* chk2(brr_t* ptr, BUF_TYPE bt)
    {
      if ((ptr != nullptr) && (ptr->bt() == bt)) return ptr;
      return nullptr;
    }
    utl& set_param_buf(brr_t* ptr)
    {
      this->par_buf_ = ptr;
      return *this;
    }
    utl& set_result_buf(brr_t* ptr)
    {
      this->res_buf_ = ptr;
      return *this;
    }
    int16_t exec() { return exec(""); }
    int16_t exec(cstr_t sql)
    {
      int16_t rc = SQL_SUCCESS;
      /* set parameter buffer size */
      if (rc == SQL_SUCCESS) rc = prepare_parameter_buffer();
      if (rc == SQL_SUCCESS) rc = prepare_result_buffer();
      if (rc == SQL_SUCCESS) rc = s_.prepare(sql);
      if (rc == SQL_SUCCESS && par_buf_ != nullptr) rc = par_buf_->bind(s_.handle());
      if (rc == SQL_SUCCESS && res_buf_ != nullptr) rc = res_buf_->bind(s_.handle());
      if (rc == SQL_SUCCESS) rc = s_.exec();
      return rc;
    }
    auto  handle() const { return s_.handle(); }
    str_t dump(cstr_t a_msg) const
    {
      str_t r;
      r += str_t(a_msg) + "\n";
      r += par_buf_ != nullptr ? par_buf_->dump("***par:***\n") : "";
      r += res_buf_ != nullptr ? res_buf_->dump("***res:***\n") : "";
      return r;
    }
    int16_t fetch_scroll(int16_t a_dir, uint a_len) { return s_.fetch_scroll(a_dir, a_len, false); }
    size_t  rows_read() const { return res_buf_ != nullptr ? res_buf_->occupied() : 0; }
  protected:
    int16_t prepare_parameter_buffer()
    {
      int16_t rc = SQL_SUCCESS;
      if (par_buf_ != nullptr)
      {
        auto dim = par_buf_->occupied();
        if (dim > 1) rc = s_.set_attr(SQL_ATTR_PARAMSET_SIZE, static_cast<int>(dim));
        if (rc == SQL_SUCCESS)
          rc = s_.SSA_ptr(SQL_ATTR_PARAMS_PROCESSED_PTR, &par_buf_->procesed());
        if (rc == SQL_SUCCESS) rc = s_.SSA_ptr(SQL_ATTR_PARAM_STATUS_PTR, par_buf_->s_vec());
      }
      return rc;
    }
    int16_t prepare_result_buffer()
    {
      int16_t rc = SQL_SUCCESS;
      if (res_buf_ != nullptr)
      {
        auto dim = res_buf_->size();
        if (dim > 1) { rc = s_.set_attr_l(SQL_ATTR_ROW_ARRAY_SIZE, dim); }
        if (rc == SQL_SUCCESS)
          rc = s_.SSA_ptr(SQL_ATTR_ROWS_FETCHED_PTR, res_buf_->occupied_addr());
      }
      return rc;
    }
  private:
    db::statement s_;                 //!< SQL statement associated with th esql operation
    brr_t*        par_buf_ = nullptr; //!< parameter buffer (can be null)
    brr_t*        res_buf_ = nullptr; //!< result buffer
    // int proc{};
  };
  /*....inline methods .......................................................................*/
};     // namespace db
#endif // DBGEN3_TEMPL_HPP
