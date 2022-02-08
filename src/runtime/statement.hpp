/*!
 * \file
 * \brief Declaration of class db::statement
 */
#ifndef STATEMENT_H
#define STATEMENT_H

#include <string_view>
#include <vector>

#include "connection.hpp"
#include "error.hpp"
namespace db
{
  class error;
  /*!
   * \brief Declaration of CLI statement class
   *
   * \ingroup runtime
   *
   * It implements statement to be executed on database (via db2 CLI)
   */
  class statement
  {
  public:
    using str_t  = std::string;
    using cstr_t = std::string_view;
    //! @name Constructor(s) and destructor
    //@{
    explicit statement(const connection* a_db);                 //!< constructor
    statement(const connection* a_db, std::string_view an_sql); //!< constructor with sql
    statement(const statement& o) = default;                    //!< copy constructor
    statement(statement&& o)      = default;                    //! move constructor
    virtual ~statement();                                       /// destructor
    //@}
    //! @name Operator(s)
    //@{
    statement& operator=(const statement& o) = delete; //!< Assignment operator
    statement& operator=(statement&& o) = delete;      //!< move assignement operator
    // statement* assign(const statement& o);             //!< assign attributes
    //@}
    //! @name Getters
    //@{
    std::int32_t      handle() const;                       //!< fetch statement handle
    cstr_t            sql() const;                          //!< fetch SQL statement
    std::string       cursor_name() const;                  //!< fetch cursor name
    const connection& db() const;                           //!< fetch db related connection
    std::string       dump(const std::string& a_msg) const; //!< serialize instance to string
    //@}
    //! @name Setters
    //@{
    void set_sql(cstr_t an_sql); //!< set sql statement
    //@}
    /*! @name Statement attribute setters
     * The setters set attributes of the statement. It is always
     * attribute code followed by a value. Values are:
     * - unsigned integers
     * - pointer to strings of characters
     * - pointer to binary buffers
     *
     * The following attribute code's are supported (db2 v9r7, check IBM documentation):
     * - SQL_ATTR_PARAM_BIND_OFFSET_PTR
     * - SQL_ATTR_PARAM_BIND_TYPE
     * - SQL_ATTR_PARAM_OPERATION_PTR
     * - SQL_ATTR_PARAM_STATUS_PTR
     * - SQL_ATTR_PARAMS_PROCESSED_PTR
     * - SQL_ATTR_PARAMSET_SIZE
     * - SQL_ATTR_ROW_ARRAY_SIZE
     * - SQL_ATTR_ROW_BIND_OFFSET_PTR
     * - SQL_ATTR_ROW_BIND_TYPE
     * - SQL_ATTR_ROW_OPERATION_PTR
     * - SQL_ATTR_ROW_STATUS_PTR
     * - SQL_ATTR_ROWS_FETCHED_PTR
     */
    //@{
    std::int16_t SSA_ptr(int attr, void* value) const;
    std::int16_t set_attr_l(int attr, uint64_t value) const;
    std::int16_t set_attr(int attr, int16_t* value) const;
    std::int16_t set_attr(int attr, int* value) const;
    std::int16_t set_attr(int attr, SQLCHAR* value) const;
    std::int16_t set_attr(int attr, void* value, uint len) const;
    std::int16_t set_attr(int attr, int value) const;
    std::int16_t set_attr(int attr, std::size_t* value) const;
    //@}
    //! @name database operations
    //@{
    /// execute an a query
    std::int16_t exec_direct(cstr_t an_sql, bool should_throw);
    std::int16_t exec() const;                //!< execute an a query with pre-loaded sql statement
    std::int16_t prepare(cstr_t an_sql = ""); //!< prepare the statement
    std::int16_t commit() const;              //!< commit statements
    std::int16_t rollback() const;            //!< rollback statements
    /// fetch buffer set
    std::int16_t fetch_scroll(int16_t a_dir, uint a_len, bool should_throw = true) const;
    /// fetch buffer set and throw only for unallowed return codes
    int fetch_with_codes(int16_t a_dir, const std::string& allowed_codes, uint a_num_of_records);
    int close_cursor() const; //!< close cursor on statement (for selects only)
    //@}
    //! @name auxiliary methods
    //@{
    /// It checks the return status of the cli call and raises exception upon error otherwise
    /// returns SQL_SUCCESS
    std::int16_t chk(std::int16_t err_code,
                     cstr_t       ok_msg,
                     cstr_t       err_msg,
                     bool         should_throw = true) const;
    /// check if code is on the list and throw accordingly if not
    int handle_return_code(int rc, cstr_t allowed_codes);
    /// set the result set current position
    virtual std::int16_t set_pos(size_t a_rec_pos);
    /// set result set current position
    virtual std::int16_t set_page(size_t a_page_num, size_t a_page_size);
    //@}
  protected:
    void log(const std::string& msg) const; //!< logs an event into log file
    /// the method prepare error string and logs it into log file
    /// std::string build_log_error(const char* error_prefix);
  private:
    const connection* db_;          //!< related database connection (no ownership)
    std::string       sql_;         //!< sql statement string
    std::int32_t      handle_ = 0;  //!< handle of the statement
    std::string       cursor_name_; //!< cursor name
    mutable bool      is_prepared_; //!< indicator whether the statement is already prepared
  };
  /// logs an event into log file
  inline void statement::log(const std::string& msg) const { db_->log(msg); }
} // namespace db
#endif
