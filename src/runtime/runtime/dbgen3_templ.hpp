#ifndef DBGEN3_TEMPL_HPP
#define DBGEN3_TEMPL_HPP

#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <span>
#include <vector>
#include <string_view>

#include "odbc_db.hpp"
#include "serialize_templ.hpp"
#include "statement.hpp"
namespace db
{
  using cstr_t     = std::string_view;
  using str_t      = std::string;
  using bstr_t     = std::span<uint8_t>;
  using cbstr_t    = std::span<const uint8_t>;
  using cstr_vec_t = std::vector<cstr_t>;

  // struct attr_data
  // {
  //   cstr_t      name{};           //!< name of the attribute
  //   std::size_t width{};          //!< width of the attribute in bytes (string => +1)
  //   uint8_t     dec{};            //!< number of decimal places or fraction in timestamp
  //   uint        db_type{};        //!< attribute type id e.g. SQL_BIGINT -> -5
  // } __attribute__((aligned(32))); // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)


  /*....root class for all attributes ........................................................*/
  template <std::size_t arr_size>
  class attr_root_root
  {
  public:
    attr_root_root()                                     = default;
    virtual ~attr_root_root()                            = default;
    attr_root_root(const attr_root_root&)                = default;
    attr_root_root(attr_root_root&&) noexcept            = default;
    attr_root_root& operator=(const attr_root_root&)     = default;
    attr_root_root& operator=(attr_root_root&&) noexcept = default;

    using l_vec = std::array<int32_t, arr_size>;
    virtual str_t                 dump_all(cstr_t a_msg, std::size_t max_el, uint offs) const = 0;
    virtual int16_t               bind_par(std::int32_t a_stmt_handle, std::int16_t a_ndx)    = 0;
    virtual int16_t               bind_col(std::int32_t a_stmt_handle, std::int16_t a_ndx)    = 0;
    constexpr virtual std::size_t size() const                                                = 0;
    virtual l_vec&                len()                                                       = 0;
    virtual const l_vec&          len() const                                                 = 0;
    // name of the parameter or result column
    //    virtual cstr_t                name() const = 0;
  };
  /**
   * @brief root class for all attributes within the buffers
   *
   */
  template <typename T, int16_t DB_TYPE, std::size_t arr_size, std::size_t DEC>
  class attr_root : public attr_root_root<arr_size>
  {
  public:
    using t_vec  = std::array<T, arr_size>;
    using l_vec  = std::array<int32_t, arr_size>;
    using cstr_t = std::string_view;
    constexpr attr_root() { std::fill(len_.begin(), len_.end(), SQL_NULL_DATA); }
    virtual ~attr_root()                                  = default;
    attr_root(const attr_root&)                           = default;
    attr_root(attr_root&&) noexcept                       = default;
    attr_root&            operator=(const attr_root&)     = default;
    attr_root&            operator=(attr_root&&) noexcept = default;
    constexpr std::size_t size() const final { return static_cast<std::size_t>(arr_size); }
    l_vec&                len() final { return len_; }
    const l_vec&          len() const final { return len_; }
    t_vec&                data() { return data_; }
    const t_vec&          data() const { return data_; }
    constexpr int32_t     width() { return width_; }
    constexpr int16_t     dec() const { return dec_; }
    constexpr int16_t     db_type() const { return db_type_; }
    int32_t               len(std::size_t ndx) const { return len_.at(ndx); }
    void                  set_len(uint32_t v, std::size_t ndx) const { len_.at(ndx) = v; }
    void                  set_null(std::size_t ndx) { len_.at(ndx) = SQL_NULL_DATA; }
    bool                  is_null(std::size_t ndx) const { return len_.at(ndx) == SQL_NULL_DATA; }
    /// size of specific element
    size_t el_size(std::size_t ndx) const { return static_cast<size_t>(len_.at(ndx)); }
    /// value getters
    bstr_t v_bstr(std::size_t ndx)
    { // be careful this can be altered from the outside
      if constexpr (is_bstring(db_type_)) { return bstr_t{data_.at(ndx).data(), el_size(ndx)}; }
    };
    cbstr_t v_bstr(std::size_t ndx) const
    {
      if constexpr (is_bstring(db_type_))
      { // must change uint8_t -> const uint8_t so that it can not be altered from outside
        // NOLINTNEXTLINE clang-tidy(cppcoreguidelines-pro-type-reinterpret-cast)
        cbstr_t r(reinterpret_cast<const uint8_t*>(&data_.at(ndx).at(0)), el_size(ndx));
        return r;
      }
    };
    cstr_t v_cstr(std::size_t ndx)
    {
      if constexpr (is_string(db_type_)) { return cstr_t{data_.at(ndx).data(), el_size(ndx)}; }
    };
    cstr_t v_cstr(std::size_t ndx) const
    {
      if constexpr (is_string(db_type_)) { return cstr_t{data_.at(ndx).data(), el_size(ndx)}; }
    };
    T v_T(std::size_t ndx)
    {
      if constexpr (is_atomic(db_type_)) { return data_.at(ndx); }
    };
    T v_T(std::size_t ndx) const
    {
      if constexpr (is_atomic(db_type_)) { return data_.at(ndx); }
    };
    void s_T(const T& v, std::size_t ndx)
    {
      if constexpr (is_atomic(db_type_))
      {
        data_.at(ndx) = v;
        len_.at(ndx)  = sizeof(T);
      }
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
    /**
     * @brief serialize contents of the instance into stirng
     *
     * @param a_msg string to prefix the serialized instance content
     * @param max_el how many values do we display (0== all)
     * @param offs offset from the left border
     * @return str_t prefixed serialized instance content
     */
    str_t dump_all(cstr_t a_msg, std::size_t max_el, uint offs) const override
    {
      std::string s;
      if (max_el == 0) max_el = arr_size;
      s = std::string(offs, ' ') + std::string(a_msg) + "[";
      for (auto cnt = 0UL; cnt < std::min(arr_size, max_el); ++cnt)
      {
        if (is_null(cnt))
        {
          s += "NULL, ";
          continue;
        }
        auto grp = attr_type(db_type_);
        switch (grp)
        {
        case db::ATTR_TYPE::atomic:
          if constexpr (is_atomic(db_type_))
          {
            s += serialize<T>(v_T(cnt));
            break;
          }
        case db::ATTR_TYPE::string:
          if constexpr (is_string(db_type_))
          {
            s += serialize<cstr_t>(v_cstr(cnt));
            break;
          }
        case db::ATTR_TYPE::bstring:
          if constexpr (is_bstring(db_type_))
          {
            s += serialize<cbstr_t>(v_bstr(cnt));
            break;
          }
        case db::ATTR_TYPE::unknown:
          throw std::runtime_error("switch error: " + std::string(__FILE__) + " " + std::to_string(__LINE__));
        }
        s += ", ";
      }
      s.resize(s.size() - 2);
      s += "]";
      return s;
    }
    /**
     * @brief bind attribute to statement
     *
     * @param a_stmt_handle statement to bind to
     * @param a_ndx attribute index within the parameter the parameter list 1..n
     * @return int16_t
     */
    int16_t bind_par(std::int32_t a_stmt_handle, std::int16_t a_ndx) override
    {
      // std::cerr << ct_name(db_type_) << "--" << dbt_name(db_type_) << "-- " << width() << "--"
      //           << dec_ << std::endl;
      auto rc = SQLBindParameter(a_stmt_handle,
                                 a_ndx,
                                 SQL_PARAM_INPUT,
                                 c_type(db_type_),
                                 db_type_,
                                 width() - 1,
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
    cstr_t name() const { return this->name_; }
  protected:
  private:
    l_vec                          len_;                 //!< value length
    t_vec                          data_{};              //!< values
    constinit static const int32_t width_   = sizeof(T); //!< parameter width
    constinit static const int16_t db_type_ = DB_TYPE;   //!< parameter type
    constinit static const int16_t dec_     = DEC;       //!< decimal places
    //    constexpr static const char*   name_;                //!< name of the attribute
  };
  /*....atomic...............................................................................*/
  template <typename T, int16_t DB_TYPE, std::size_t arr_size, int DEC>
  class atomic : public attr_root<T, DB_TYPE, arr_size, DEC>
  {
  public:
    using t_vec        = std::array<T, arr_size>;
    using l_vec        = std::array<int32_t, arr_size>;
    using PP           = attr_root<T, DB_TYPE, arr_size, DEC>;
    constexpr atomic() = default;
    T    value() const { return value(0); }
    T    value(std::size_t ndx) const { return PP::data().at(ndx); }
    void set_value(const T& data) { set_value(data, 0); }
    //    void    set_value(const T& data, std::size_t ndx) { PP::data().at(ndx) = data; }
    void    set_value(const T& data, std::size_t ndx) { PP::s_T(data, ndx); }
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
  template <typename T, int16_t DB_TYPE, std::size_t arr_size, typename EL>
  class bstring : public attr_root<T, DB_TYPE, arr_size, 0>
  {
  public:
    using PP            = attr_root<T, DB_TYPE, arr_size, 0>;
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
  template <typename T, int16_t DB_TYPE, std::size_t arr_size, std::size_t DEC, typename EL>
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
  ///////////////////////////////////////////////////////////////////////////////////////////////
  /**
   * @brief buffer root class as seen from programmer (no dimennsion only pure methods)
   *
   */
  class buffer_root_root // NOLINT cppcoreguidelines-virtual-class-destructor
  {
  public:
    using s_vec_a  = std::span<SQLUSMALLINT>;
    using span_str = std::span<const cstr_t>;

    virtual str_t              dump(cstr_t a_msg) const           = 0;
    virtual int16_t            bind(std::int32_t a_stmt_handle)   = 0;
    virtual int16_t            rebind(std::int32_t a_stmt_handle) = 0;
    virtual size_t             size() const                       = 0;
    virtual size_t             occupied() const                   = 0;
    virtual void               set_occupied(size_t v)             = 0;
    virtual size_t*            occupied_addr()    = 0; // so that cli can write how many records are read
    virtual constexpr BUF_TYPE bt()               = 0;
    virtual span_str           attr_names() const = 0;
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
    using attr_vec_t                               = std::vector<attr_root_root<arr_size>*>;
    using s_vec_t                                  = std::array<SQLUSMALLINT, arr_size>;
    using s_vec_a                                  = std::span<SQLUSMALLINT>;
    constexpr buffer_root()                        = default;
    ~buffer_root()                                 = default;
    buffer_root(const buffer_root&)                = default;
    buffer_root(buffer_root&&) noexcept            = default;
    buffer_root& operator=(const buffer_root&)     = default;
    buffer_root& operator=(buffer_root&&) noexcept = default;
    std::size_t  size() const override { return arr_size; }
    void*        s_vec() override { return s_vec_.data(); }
    const void*  s_vec() const override { return s_vec_.data(); }
    std::size_t& procesed() override { return processed_; }
    std::size_t  processed() const override { return processed_; }

    str_t dump(cstr_t a_msg) const override
    {
      str_t r;
      r += str_t(a_msg) + "\n";
      r += "# attr:   " + std::to_string(attributes().size()) + "\n";
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
      auto cnt     = 0;
      auto names   = attr_names();
      auto max_len = 0UL;
      // calc max column name length
      for (const auto& el : names)
        if (max_len < el.size()) max_len = el.size();
      for (const auto& el : attributes())
      {
        const auto name = std::string(names[cnt]) + std::string(max_len - names[cnt].size(), ' ');
        r += el->dump_all(name, occupied(), 0) + "\n";
        cnt++;
      }
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
    size_t             occupied() const override { return occupied_; }
    void               set_occupied(size_t v) override { occupied_ = v; }
    size_t*            occupied_addr() override { return &occupied_; }
    constexpr BUF_TYPE bt() override { return buffer_root::bt_; }
  protected:
    virtual std::vector<db::attr_root_root<arr_size>*>&       attributes()       = 0;
    virtual const std::vector<db::attr_root_root<arr_size>*>& attributes() const = 0;
  private:
    int16_t bind_par(std::int32_t a_stmt_handle)
    {
      if constexpr (bt_ == BUF_TYPE::par)
      {
        int16_t rc  = SQL_SUCCESS;
        auto    ndx = 0;
        for (const auto& el : attributes())
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
        for (const auto& el : attributes())
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
    constexpr static BUF_TYPE bt_ = BT;     //!< buffer type
    size_t                    occupied_{};  //!< how many records is occupied [0..size())
    bool                      bind_{};      //!< are the parameters bound to the statement?
    s_vec_t                   s_vec_{};     //!< vector of statuses
    size_t                    processed_{}; //!< number of parameters processed
  };
  ///////////////////////////////////////////////////////////////////////////////////////////
  class utl // NOLINT cppcoreguidelines-virtual-class-destructor
  {
  public:
    using brr_t    = buffer_root_root;
    using int_span = std::span<const int>;
    utl()          = delete;
    utl(db::connection* c, cstr_t sql)
    : utl(c, sql, nullptr, nullptr) // no parameters or results
    { }
    utl(db::connection* c, cstr_t sql, brr_t* ptr) // parameter or result
    : utl(c, sql, chk2(ptr, BUF_TYPE::par), chk(ptr, "You must provide proper buffer object generated by dbgen3."))
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
        throw std::runtime_error(static_cast<str_t>(a_msg));
      }
      return nullptr;
    }
    static brr_t* chk2(brr_t* ptr, BUF_TYPE bt)
    {
      if ((ptr != nullptr) && (ptr->bt() == bt)) return ptr;
      return nullptr;
    }
    void                 set_par_buf(brr_t* ptr) { this->par_buf_ = ptr; }
    void                 set_res_buf(brr_t* ptr) { this->res_buf_ = ptr; }
    virtual brr_t*       par_buf() { return this->par_buf_; }
    virtual const brr_t* par_buf() const { return this->par_buf_; }
    virtual brr_t*       res_buf() { return this->res_buf_; }
    virtual const brr_t* res_buf() const { return this->res_buf_; }
    /**
     * @brief default prepared query execution
     *
     * @return int16_t
     */
    int16_t exec() { return exec(""); }
    /**
     * @brief execute the prepared query
     *
     * The method configures the parameter and/or result variables, prepare and bind the
     * sql statement to buffer(s) nad execute the statement.
     * upon completition it returns the operation return code.
     * if code is listed upon "allowrd codes" it return such code, for unknown code it raises an
     * exception.
     *
     * @param sql sql statement to be executed
     * @return int16_t result code
     */
    int16_t exec(cstr_t sql)
    {
      rc_ = SQL_SUCCESS;
      /* set parameter buffer size */
      if (rc_ == SQL_SUCCESS) rc_ = prepare_parameter_buffer();
      if (rc_ == SQL_SUCCESS) rc_ = prepare_result_buffer();
      if (rc_ == SQL_SUCCESS) rc_ = s_.prepare(sql);
      if (rc_ == SQL_SUCCESS && par_buf_ != nullptr) rc_ = par_buf_->bind(s_.handle());
      if (rc_ == SQL_SUCCESS && res_buf_ != nullptr) rc_ = res_buf_->bind(s_.handle());
      if (rc_ == SQL_SUCCESS) rc_ = s_.exec();
      if (rc_ != SQL_SUCCESS) return s_.handle_return_code(rc_, allowed_codes());
      return SQL_SUCCESS; // ni pravo mesto, mora biti na na q
    }
    /**
     * @brief return statement handle
     *
     * @return auto statement handle
     */
    auto handle() const { return s_.handle(); }
    /**
     * @brief serialize instance contents
     *
     * @param a_msg message to prefix the serialized content
     * @return str_t serialized contents of the instance
     */
    str_t dump(cstr_t a_msg) const
    {
      str_t r;
      r += str_t(a_msg) + "\n";
      r += par_buf_ != nullptr ? par_buf_->dump("-->par:***\n") : "";
      r += res_buf_ != nullptr ? res_buf_->dump("-->res:***\n") : "";
      return r;
    }
    /**
     * @brief fech next block of records
     *
     * @return int16_t usually SQL_SUCCESS and SQL_NO_DATA
     */
    int16_t fetch()
    {
      if (res_buf_ != nullptr)
      {
        rc_ = s_.fetch_scroll(SQL_FETCH_NEXT, res_buf_->size(), false);
        if (rc_ == SQL_NO_DATA) return rc_; // end of the result set
        return s_.handle_return_code(rc_, allowed_codes());
      }
      throw std::runtime_error("error - fetch and no result buffer defined.");
    }
    size_t  rows_read() const { return res_buf_ != nullptr ? res_buf_->occupied() : 0; }
    int16_t rc() const { return this->rc_; }
  protected:
    /**
     * @brief set the parameter buffer related variables
     *
     * @return int16_t
     */
    int16_t prepare_parameter_buffer()
    {
      rc_ = SQL_SUCCESS;
      if (par_buf_ != nullptr)
      {
        auto dim = par_buf_->occupied();
        if (dim > 1) rc_ = s_.set_attr(SQL_ATTR_PARAMSET_SIZE, static_cast<int>(dim));
        if (rc_ == SQL_SUCCESS) rc_ = s_.SSA_ptr(SQL_ATTR_PARAMS_PROCESSED_PTR, &par_buf_->procesed());
        if (rc_ == SQL_SUCCESS) rc_ = s_.SSA_ptr(SQL_ATTR_PARAM_STATUS_PTR, par_buf_->s_vec());
      }
      return rc_;
    }
    /**
     * @brief set the result buffer variables
     *
     * @return int16_t
     */
    int16_t prepare_result_buffer()
    {
      rc_ = SQL_SUCCESS;
      if (res_buf_ != nullptr)
      {
        auto dim = res_buf_->size();
        if (dim > 1) { rc_ = s_.set_attr_l(SQL_ATTR_ROW_ARRAY_SIZE, dim); }
        if (rc_ == SQL_SUCCESS) rc_ = s_.SSA_ptr(SQL_ATTR_ROWS_FETCHED_PTR, res_buf_->occupied_addr());
      }
      return rc_;
    }
    /**
     * @brief list of return codes that do not raise exception
     *
     * @return std::vector<int>
     */
    virtual std::vector<int> allowed_codes() const = 0;
  private:
    db::statement s_;                     //!< SQL statement associated with th esql operation
    brr_t*        par_buf_ = nullptr;     //!< parameter buffer (can be null)
    brr_t*        res_buf_ = nullptr;     //!< result buffer (can be null)
    int16_t       rc_      = SQL_SUCCESS; //!< return codes
  };

  // constexpr static const std::array<attr_data, 5> arr;

  // constexpr cstr_t a_name(uint ndx, const std::span<attr_data> a)
  // {
  //   if (ndx < a.size()) { return (a.begin() + ndx)->name; }
  //   throw std::runtime_error("a_name index out of scope");
  // }
  // constexpr std::size_t a_width(size_t ndx, std::span<const db::attr_data> a)
  // {
  //   if (ndx < a.size()) { return (a.begin() + ndx)->width; } // NOLINT
  //   throw std::runtime_error("a_width index out of scope");
  // }


  /*....inline methods .......................................................................*/
};     // namespace db
#endif // DBGEN3_TEMPL_HPP
