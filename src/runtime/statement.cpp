/*!
 * \file
 * \brief Implementation of class db::statement
 *
 */
#include <sqlcli1.h>
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
  : statement(a_db, "")
  { }
  /// constructor with sql
  statement::statement(const connection* a_db, std::string_view an_sql)
  : db_(a_db)
  , sql_(an_sql)
  , handle_(db_->alloc_stmt_handle())
  , is_prepared_(false)
  { }
  /*!
   * destructor
   *
   * Throw is intentionally commented out.
   * Never throw exception in destructor.
   */
  statement::~statement()
  {
    db::connection::dealloc_stmt_handle(handle_);
    log(str_t("Statement disposed: " + std::to_string(handle_) + " sql='") + str_t(sql()) +
        str_t("'."));
  }
  /*!
   * Execute direct sql statement
   * \param an_sql an sql statement to be executed
   * \returns return code of the operation 0 == ok
   */
  std::int16_t statement::exec_direct(cstr_t an_sql, bool should_throw)
  {
    if (! an_sql.empty()) set_sql(an_sql);
    std::int16_t sts =
      chk(SQLExecDirect(handle_,
                        reinterpret_cast<SQLCHAR*>(const_cast<char*>(sql().data())), // NOLINT
                        SQL_NTS),
          str_t("exec direct ok    :- sql='") + str_t(sql()) + "'.",
          str_t("exec direct failed:- sql='") + str_t(sql()) + "'.",
          should_throw);
    return sts;
  }
  /*!
   * Execute predefined SQL statement
   * \returns return code of the operation 0 == ok
   */
  std::int16_t statement::exec() const
  {
    return chk(SQLExecute(handle_),
               str_t("exec ok    :- sql='") + str_t(sql()) + "'.",
               str_t("exec failed:- sql='") + str_t(sql()) + "'.",
               false);
  }
  /*!
   * Prepare the statement
   * \param an_sql an sql statement to be prepared
   * \returns return code of the operation 0 == ok
   */
  std::int16_t statement::prepare(cstr_t an_sql)
  {
    std::int16_t rc = SQL_SUCCESS;
    if (! an_sql.empty()) set_sql(an_sql);
    if (! is_prepared_)
    {
      auto ok_ans  = str_t("prepare ok :- sql='") + str_t(sql()) + str_t("'.");
      auto nak_ans = str_t("prepare failed:- sql='") + str_t(sql()) + str_t("'.");

      rc = chk(
        SQLPrepare(handle_,
                   reinterpret_cast<SQLCHAR*>( // NOLINT cppcoreguidelines-pro-type-reinterpret-cast
                     const_cast<char*>( // NOLINT -cppcoreguidelines-pro-type-reinterpret-cast
                       sql().data())),
                   SQL_NTS),
        ok_ans,
        nak_ans);
      is_prepared_ = rc == SQL_SUCCESS;
    }
    else log(str_t("Already prepared: '") + str_t(sql()) + str_t("'."));
    return rc;
  }
  /**
   * The method serialises content of the object attributes
   * to the stream.
   */
  std::string statement::dump(const std::string& a_msg) const
  {
    std::stringstream s;
    s << a_msg << std::endl
      << " sql:'" << sql() << "'" << std::endl
      << " stmt:   " << handle() << std::endl
      << " cursor:'" << cursor_name() << "'" << std::endl
      << " conn:   " << db().dump();
    return s.str();
  }
  /// fetch statement handle
  SQLHANDLE statement::handle() const { return this->handle_; }
  /// commit statements
  std::int16_t statement::commit() const { return db_->commit(); }
  /// rollback statements
  std::int16_t statement::rollback() const { return db_->rollback(); }
  /// fetch buffer set
  /*!
   * The method fetches a record set. Records are taken either relative or absolute.
   * @param a_dir    - direction of the fetch (see SQLFetchScroll constants)
   * @param a_len    -  number of records to fetch
   * @param should_throw - should the method throws an exception if error code is not SQL_SUCCESS
   * (i.e. 0)
   *        - true - throw an exception if status of the operation is not equal to 0
   *        - false - return only return code; no exception
   * @return error code of the fetch operation
   * \throws db::error_exception should the status code is not 0 and should_thow is true
   */
  std::int16_t statement::fetch_scroll(int16_t a_dir, uint a_len, bool should_throw) const
  {
    return chk(SQLFetchScroll(handle_, a_dir, static_cast<std::make_signed_t<uint>>(a_len)),
               str_t("fetch scroll ok    :- sql='") + str_t(sql()) + "'.",
               str_t("fetch scroll failed:- sql='") + str_t(sql()) + "'.",
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
  std::int16_t statement::chk(std::int16_t err_code,
                              cstr_t       ok_msg,
                              cstr_t       err_msg,
                              bool         should_throw) const
  {
    if (err_code != SQL_SUCCESS)
    {
      error err;
      err.load(handle_, SQL_HANDLE_STMT);
      std::string str = str_t(err_msg) + str_t(" : '") + err.dump(str_t(sql()) + "' ##  err:");
      log(str);
      if (should_throw) throw error_exception(str);
      //      log("Error code: " + std::to_string(err_code));
    }
    else log(str_t(ok_msg));
    return err_code;
  }
  /// fetch an sql statement
  inline cstr_t statement::sql() const { return sql_; }
  /// set sql statement
  void statement::set_sql(cstr_t an_sql)
  {
    if (sql() != an_sql)
    {
      sql_         = an_sql;
      is_prepared_ = false;
      log(str_t("sql set:'") + str_t(sql()) + str_t("'"));
    }
    // we don't need else branch - removed on purpose
  }
  /// fetch the related connecton object
  const connection& statement::db() const { return *db_; }
  /// fetch cursor name
  std::string statement::cursor_name() const { return cursor_name_; }
  /*!
   * The method fetches record set. The operation can return various codes. Parameter allowed_codes
   * contains a list of codes which are valid in the provided context. For this set the method
   * return status code, for all others it throws an exception.
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
  int statement::close_cursor() const { return SQLCloseCursor(this->handle_); }
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
  int statement::handle_return_code(int rc, cstr_t allowed_codes)
  { // FIXME should be binary vector
    std::string fnc(&__func__[0]);
    if (rc != SQL_SUCCESS)
    {
      error err;
      err.load(handle_, SQL_HANDLE_STMT);

      int code = err.get_error_code(0);
      if (str_t(allowed_codes).find(std::to_string(code)) == std::string::npos)
      {
        std::string err_str = err.dump(sql());
        log(fnc + str_t(":") + err_str);
        throw error_exception(err_str);
      };
    }
    log(fnc + str_t(": OK"));
    return rc;
  }
  /*!
   * The method set the statement attribute as unsigned integer value.
   * @param attr attribute name/code
   * @param value attribute value
   * @return return code of the operation
   */
  std::int16_t statement::set_attr_l(int attr, uint64_t value) const
  {
    std::int16_t ret = SQLSetStmtAttr(handle_,
                                      attr,
                                      reinterpret_cast<void*>(value), // NOLINT
                                      0);
    log(str_t("status:") + std::to_string(ret) + //
        " attribute:'" + std::to_string(attr) +  //
        "' value:'" + std::to_string(value) +    //
        "'.");
    return ret;
  }
  std::int16_t statement::set_attr(const int attr, int16_t* value) const
  {
    std::int16_t ret = SQLSetStmtAttr(handle_, attr, static_cast<SQLPOINTER>(value), 0);
    log(str_t("status:") + std::to_string(ret) + " attribute:'" + std::to_string(attr) +
        "' value:'" + std::to_string(*value) + "'.");
    return ret;
  }
  std::int16_t statement::set_attr(const int attr, int* value) const
  {
    std::int16_t ret = SQLSetStmtAttr(handle_, attr, static_cast<SQLPOINTER>(value), 0);
    log(str_t("status:") + std::to_string(ret) + " attribute:'" + std::to_string(attr) +
        "' value:'" + std::to_string(*value) + "'.");
    return ret;
  }
  std::int16_t statement::set_attr(int attr, std::size_t* value) const
  {
    std::int16_t ret = SQLSetStmtAttr(handle_, attr, static_cast<SQLPOINTER>(value), 0);
    log(str_t("status:") + std::to_string(ret) + " attribute:'" + std::to_string(attr) +
        "' value:'" + std::to_string(*value) + "'.");
    return ret;
  }
  std::int16_t statement::set_attr(const int attr, SQLCHAR* value) const
  {
    std::int16_t ret = SQLSetStmtAttr(handle_,
                                      attr,
                                      reinterpret_cast<void*>(value), // NOLINT
                                      SQL_NTS);
    log(str_t("status:") + std::to_string(ret) + " attribute:'" + std::to_string(attr) +
        "' value:'" + std::to_string(*value) + "'.");
    return ret;
  }
  std::int16_t statement::set_attr(const int attr, void* value, const unsigned int len) const
  {
    std::int16_t ret = SQLSetStmtAttr(handle_,
                                      attr,
                                      reinterpret_cast<SQLPOINTER>(value), // NOLINT
                                      SQL_LEN_BINARY_ATTR(len));
    log(str_t("status:") + std::to_string(ret) + " attribute:'" + std::to_string(attr) +
        "' value:'" +
        str_t(reinterpret_cast<const char*>(value), // NOLINT
              len) +
        "'.");
    // log(std::format("status: {} attribute:'{}' value:'{}'.", ret, attr, value));
    return ret;
  }
  std::int16_t statement::set_attr(const int attr, int value) const
  {
    std::int16_t ret = SQLSetStmtAttr(
      handle_,
      attr,
      reinterpret_cast<SQLPOINTER>(value), // NOLINT -cppcoreguidelines-pro-type-reinterpret-cast
      0);
    log(str_t("status:") + std::to_string(ret) + " attribute:'" + std::to_string(attr) +
        "' value:'" + std::to_string(value) + "'.");
    return ret;
  }
  std::int16_t statement::SSA_ptr(int attr, void* value) const
  {
    return SQLSetStmtAttr(handle_, attr, value, SQL_IS_POINTER);
  }
  /*!
   * The method repositions the current record position within the result set to the
   * provided record position.
   * @param a_rec_pos record position within the result set we would like to set.
   *        First record position is 1.
   */
  std::int16_t statement::set_pos(size_t a_rec_pos)
  {
    return SQLSetPos(handle_, a_rec_pos, SQL_POSITION, SQL_LOCK_NO_CHANGE);
  }
  /*!
   * The method position itself to the beginning of the provided page.
   * @param a_page_num page number to position to. Pages are 0 based.
   * @param a_page_size size of one page (in records)
   */
  std::int16_t statement::set_page(size_t a_page_num, size_t a_page_size)
  {
    return set_pos(a_page_num * a_page_size + 1);
  }
} // namespace db
