/*!
 * \file
 * \brief Implementation of class db::statement
 *
 */


//#include <iostream>

#include <sstream>
#include <stdexcept>
#include <string>

#include "error_exception.hpp"
#include "statement.hpp"


//#include "fmt/core.h"

namespace db
{
  /**
   * Constructor
   */
  statement::statement(const connection* a_db)
  : m_db(a_db)
  , m_handle(m_db->get_free_handle())
  , is_prepared(false)
  {
  }
  /// constructor with sql
  statement::statement(const connection* a_db, const char* an_sql)
  : m_db(a_db)
  , m_sql(an_sql)
  , m_handle(m_db->get_free_handle())
  , is_prepared(false)
  {
  }
  /*!
   * destructor
   *
   * Throw is intentionally commented out.
   * Never throw exception in destructor.
   */
  statement::~statement()
  {
    log(std::string("disposing statement: sql='") + get_sql() + std::string("'."));
    m_db->release_stmt_handle(m_handle);
    log("Statement disposed.");
  }
  /*!
   * Execute direct sql statement
   * \param an_sql an sql statement to be executed
   * \returns return code of the operation 0 == ok
   */
  SQLRETURN statement::exec_direct(const std::string& an_sql, bool should_throw)
  {
    set_sql(an_sql);
    SQLRETURN sts =
      chk(SQLExecDirect(m_handle,
                        reinterpret_cast<SQLCHAR*>(const_cast<char*>(an_sql.data())), // NOLINT
                        SQL_NTS),
          std::string("exec direct ok    :- sql='") + get_sql() + "'.",
          std::string("exec direct failed:- sql='") + get_sql() + "'.",
          should_throw);
    return sts;
  }
  /*!
   * Execute predefined SQL statement
   * \returns return code of the operation 0 == ok
   */
  SQLRETURN statement::exec() const
  {
    SQLRETURN sts = chk(SQLExecute(m_handle),
                        std::string("exec ok    :- sql='") + get_sql() + "'.",
                        std::string("exec failed:- sql='") + get_sql() + "'.",
                        false);
    return sts;
  }
  /*!
   * Prepare the statement
   * \param an_sql an sql statement to be prepared
   * \returns return code of the operation 0 == ok
   */
  SQLRETURN statement::prepare(const std::string& an_sql)
  {
    SQLRETURN rc = SQL_SUCCESS;
    if (an_sql.empty()) m_sql = an_sql;
    if (! is_prepared)
      {
        auto ok_ans  = std::string("prepare ok :- sql='") + m_sql + std::string("'.");
        auto nak_ans = std::string("prepare failed:- sql='") + m_sql + std::string("'.");

        rc          = chk(SQLPrepare(
                   m_handle,
                   reinterpret_cast<SQLCHAR*>( // NOLINT cppcoreguidelines-pro-type-reinterpret-cast
                     const_cast<char*>(        // NOLINT -cppcoreguidelines-pro-type-reinterpret-cast
                       m_sql.data())),
                   SQL_NTS),
                 ok_ans,
                 nak_ans);
        is_prepared = rc == SQL_SUCCESS;
      }
    else log(std::string("Already prepared: '") + m_sql + std::string("'."));
    return rc;
  }
  /**
   * The method serialises content of the object attributes
   * to the stream.
   */
  std::ostream& operator<<(std::ostream& s, const statement& o)
  {
    s << " sql:'" << o.get_sql() << "'" << std::endl
      << " stmt:   " << o.get_stmt_handle() << std::endl
      << " cursor:'" << o.get_cursor_name() << "'" << std::endl
      << " conn:   " << o.get_db();
    return s;
  }
  /// fetch statement handle
  SQLHANDLE statement::get_stmt_handle() const { return this->m_handle; }
  /// commit statements
  SQLRETURN statement::commit() const { return m_db->commit(); }
  /// rollback statements
  SQLRETURN statement::rollback() const { return m_db->rollback(); }
  /// fetch buffer set
  /*!
   * The method fetches a record set. Records are taken either relative or absolute.
   * @param a_dir    - direction of the fetch (see SQLFetchScroll constants)
   * @param a_len    -  number of records to fetch
   * @param should_throw - should the method throws an exception if error code is not SQL_SUCCESS (i.e. 0)
   *        - true - throw an exception if status of the operation is not equal to 0
   *        - false - return only return code; no exception
   * @return error code of the fetch operation
   * \throws db::error_exception should the status code is not 0 and should_thow is true
   */
  SQLRETURN statement::fetch_scroll(int16_t a_dir, uint a_len, bool should_throw) const
  {
    return chk(SQLFetchScroll(m_handle, a_dir, static_cast<std::make_signed_t<uint>>(a_len)),
               std::string("fetch scroll ok    :- sql='") + get_sql() + "'.",
               std::string("fetch scroll failed:- sql='") + get_sql() + "'.",
               should_throw);
  }
  /*!
   * The methods checks the return status of the cli call and raises exception
   * upon error otherwise returns SQL_SUCCESS
   * \param err_code return status of CLI call
   * \param ok_msg   message to be written into a log upon successful finish
   * \param err_msg  message to be written into a log upon error
   * \param should_throw true - on error it throws an exception
   *                     false - on error just return error code (i.e. no exception)
   */
  SQLRETURN statement::chk(SQLRETURN          err_code,
                           const std::string& ok_msg,
                           const std::string& err_msg,
                           bool               should_throw) const
  {
    if (err_code != SQL_SUCCESS)
      {
        if (should_throw)
          {
            error err;
            err.load(m_handle, SQL_HANDLE_STMT, m_sql);
            std::string str = err_msg + std::string(" : ") + err.dump();
            log(str);
            throw error_exception(str);
          }
        log("Error code: " + std::to_string(err_code));
      }
    else {
        log(ok_msg);
      }
    return err_code;
  }
  /// fetch an sql statement
  const std::string& statement::get_sql() const { return m_sql; }
  /// set sql statement
  void statement::set_sql(const std::string& an_sql)
  {

    if (m_sql != an_sql)
      {
        m_sql       = an_sql;
        is_prepared = false;
        log(std::string("new sql:'") + m_sql + std::string("'"));
      }
    else log("No changes in sql.");
  }
  /*!
   * It fetches pointer to connection object
   * @return
   */
  const connection* statement::get_db() const { return m_db; }
  /// fetch cursor name
  std::string statement::get_cursor_name() const { return m_cursor_name; }
  /*!
   * The method fetches record set. The operation can return various codes. Parameter allowed_codes contains a list of
   * codes which are valid in the provided context. For this set the method return status code, for all others it
   * throws an exception.
   *
   * @param a_dir - SQL_FETCH_NEXT      1
   *                SQL_FETCH_FIRST     2
   *                SQL_FETCH_LAST      3
   *                SQL_FETCH_PRIOR     4
   *                SQL_FETCH_ABSOLUTE  5
   *                SQL_FETCH_RELATIVE  6
   * @param allowed_codes list of codes which are "allowed" (i.e. they do not throw an exception)
   * \param a_num_of_records number of records to be fetched
   * @return CLI return code
   * \throws db::error_exception for all non provided error codes
   */
  int statement::fetch_with_codes(int16_t            a_dir,
                                  const std::string& allowed_codes,
                                  uint               a_num_of_records)
  {

    auto rc = fetch_scroll(a_dir, a_num_of_records + 0, false);
    return handle_return_code(rc, allowed_codes);
  }
  /*!
   * close cursor on statement (for selects only)
   */
  int statement::close_cursor() const { return SQLCloseCursor(this->m_handle); }

  /*!
   * The method checks if code returned is on the provided list. If code
   * exists on the provided list it simply returns, otherwise it throws an
   * exception with description of error(s)
   *
   * @note code is not propagated further
   *
   * @param rc return code from CLI database operation
   * @param allowed_codes list of codes which do not trigger an exception
   * @return SQL_SUCCESS or one of allowed codes
   * @throws it can throw an error_exception object for each unlisted return code
   */
  int statement::handle_return_code(int rc, const std::string& allowed_codes)
  {
    std::string fnc(&__func__[0]);
    if (rc != SQL_SUCCESS)
      {
        error err;
        err.load(m_handle, SQL_HANDLE_STMT, m_sql);

        int code = err.get_error_code();
        if (allowed_codes.find(std::to_string(code)) == std::string::npos)
          {
            std::string err_str = err.dump();
            log(fnc + std::string(":") + err_str);
            throw error_exception(err_str);
          };
      }
    log(fnc + std::string(": OK"));
    return rc;
  }
  /*!
   * The method set the statement attribute as unsigned integer value.
   * @param attr attribute name/code
   * @param value attribute value
   * @return return code of the operation
   */
  SQLRETURN statement::set_attr_l(const int attr, uint64_t value) const
  {
    SQLRETURN ret = SQLSetStmtAttr(m_handle,
                                   attr,
                                   reinterpret_cast<void*>(value), // NOLINT
                                   0);
    log(std::string("status:") + std::to_string(ret) + " attribute:'" + std::to_string(attr) +
        "' value:'" + std::to_string(value) + "'.");
    return ret;
  }
  SQLRETURN statement::set_attr(const int attr, int16_t* value) const
  {

    SQLRETURN ret = SQLSetStmtAttr(m_handle, attr, static_cast<SQLPOINTER>(value), 0);
    log(std::string("status:") + std::to_string(ret) + " attribute:'" + std::to_string(attr) +
        "' value:'" + std::to_string(*value) + "'.");
    return ret;
  }
  SQLRETURN statement::set_attr(const int attr, int* value) const
  {

    SQLRETURN ret = SQLSetStmtAttr(m_handle, attr, static_cast<SQLPOINTER>(value), 0);
    log(std::string("status:") + std::to_string(ret) + " attribute:'" + std::to_string(attr) +
        "' value:'" + std::to_string(*value) + "'.");
    return ret;
  }
  SQLRETURN statement::set_attr(const int attr, SQLCHAR* value) const
  {

    SQLRETURN ret = SQLSetStmtAttr(m_handle,
                                   attr,
                                   reinterpret_cast<void*>(value), // NOLINT
                                   SQL_NTS);
    log(std::string("status:") + std::to_string(ret) + " attribute:'" + std::to_string(attr) +
        "' value:'" + std::to_string(*value) + "'.");
    return ret;
  }
  SQLRETURN statement::set_attr(const int attr, void* value, const unsigned int len) const
  {

    SQLRETURN ret = SQLSetStmtAttr(m_handle,
                                   attr,
                                   reinterpret_cast<SQLPOINTER>(value), // NOLINT
                                   SQL_LEN_BINARY_ATTR(len));
    log(std::string("status:") + std::to_string(ret) + " attribute:'" + std::to_string(attr) +
        "' value:'" +
        std::string(reinterpret_cast<const char*>(value), //NOLINT
                    len) +
        "'.");
    // log(std::format("status: {} attribute:'{}' value:'{}'.", ret, attr, value));
    return ret;
  }
  SQLRETURN statement::set_attr(const int attr, int value) const
  {

    SQLRETURN ret = SQLSetStmtAttr(
      m_handle,
      attr,
      reinterpret_cast<SQLPOINTER>(value), //NOLINT -cppcoreguidelines-pro-type-reinterpret-cast
      0);
    log(std::string("status:") + std::to_string(ret) + " attribute:'" + std::to_string(attr) +
        "' value:'" + std::to_string(value) + "'.");
    return ret;
  }
  /*!
   * The method repositions the current record position within the result set to the
   * provided record position.
   * @param a_rec_pos record position within the result set we would like to set.
   *        First record position is 1.
   */
  SQLRETURN statement::set_pos(size_t a_rec_pos)
  {
    return SQLSetPos(this->m_handle, a_rec_pos, SQL_POSITION, SQL_LOCK_NO_CHANGE);
  }
  /*!
   * The method position itself to the beginning of the provided page.
   * @param a_page_num page number to position to. Pages are 0 based.
   * @param a_page_size size of one page (in records)
   */
  SQLRETURN statement::set_page(size_t a_page_num, size_t a_page_size)
  {
    return set_pos(a_page_num * a_page_size + 1);
  }
} //namespace db