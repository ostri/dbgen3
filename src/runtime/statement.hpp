/*!
 * \file
 * \brief Declaration of class db::statement
 *
 */
#ifndef STATEMENT_H
#define STATEMENT_H

#include <vector>

#include "connection.hpp"
#include "error.hpp"

#include <sqlcli1.h>
//#include "load_dscr.hpp"
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
    //! @name Constructor(s) and destructor
    //@{
    /// constructor
    explicit statement(const connection& a_db);
    /// constructor with sql
    statement(const connection& a_db, const char* an_sql);
    /// copy constructor
    statement(const statement& o) = default;
    statement(statement&& o)      = default;
    /// destructor
    virtual ~statement();
    //@}

    //! @name Operator(s)
    //@{
    ///Assignment operator
    statement& operator=(const statement& o) = default;
    /// move assignement operator
    statement& operator=(statement&& o) = default;
    /// assign attributes
    statement* assign(const statement& o);
    //@}
    //! @name Getters
    //@{
    SQLHANDLE          get_stmt_handle() const;              //!< fetch statement handle
    const std::string& get_sql() const;                      //!< fetch SQL statement
    std::string        get_cursor_name() const;              //!< fetch cursor name
    const connection&  get_db() const;                       //!< fetch db related connection
    std::string        dump(const std::string& a_msg) const; //!< serialize instance to string
    //@}
    //! @name Setters
    //@{
    /// set sql statement
    void set_sql(const std::string& an_sql);
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
    SQLRETURN set_attr_l(int attr, uint64_t value) const;
    SQLRETURN set_attr(int attr, int16_t* value) const;
    SQLRETURN set_attr(int attr, int* value) const;
    SQLRETURN set_attr(int attr, SQLCHAR* value) const;
    SQLRETURN set_attr(int attr, void* value, uint len) const;
    SQLRETURN set_attr(int attr, int value) const;
    //@}
    //! @name database operations
    //@{
    /// execute an a query
    SQLRETURN exec_direct(const std::string& an_sql, bool should_throw);
    /// execute an a query with pre-loaded sql statement
    SQLRETURN exec() const;
    /// prepare the statement
    SQLRETURN prepare(const std::string& an_sql = "");
    // /// begin transaction
    // SQLRETURN begin_transaction() const;
    /// commit statements
    SQLRETURN commit() const;
    /// rollback statements
    SQLRETURN rollback() const;
    /// fetch buffer set
    SQLRETURN fetch_scroll(int16_t a_dir, uint a_len, bool should_throw = true) const;
    //! fetch buffer set and throw only for unallowed return codes
    int fetch_with_codes(int16_t a_dir, const std::string& allowed_codes, uint a_num_of_records);
    //! close cursor on statement (for selects only)
    int close_cursor() const;
    //@}
    //! @name auxiliary methods
    //@{
    /// It checks the return status of the cli call and raises exception upon error otherwise returns SQL_SUCCESS
    SQLRETURN chk(SQLRETURN          err_code,
                  const std::string& ok_msg,
                  const std::string& err_msg,
                  bool               should_throw = true) const;
    /// check if code is on the list and throw accordingly if not
    int handle_return_code(int rc, const std::string& allowed_codes);
    /// set the result set current position
    virtual SQLRETURN set_pos(size_t a_rec_pos);
    /// set result set current position
    virtual SQLRETURN set_page(size_t a_page_num, size_t a_page_size);
    //@}
  protected:
    /// logs an event into log file
    void log(const std::string& msg) const;
    /// the method prepare error string and logs it into log file
    std::string build_log_error(const char* error_prefix);
  private:
    const connection& db_;          //!< related database connection
    std::string       sql_;         //!< sql statement string
    SQLHANDLE         handle_ = 0;  //!< handle of the statement
    std::string       cursor_name_; //!< cursor name
    mutable bool      is_prepared_; //!< indicator whether the statement is already prepared
  };
  /// logs an event into log file
  inline void statement::log(const std::string& msg) const { db_.log(msg); }
} // namespace db

#endif
