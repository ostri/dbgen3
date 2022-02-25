/*!
 * \file
 * \brief Implementation of class db::connection
 *
 * when       who what
 * 05.08.2009 Os3 method set_attribute added
 */
#include <cstddef>
#include <mutex>
#include <stdexcept>
#include <string>

#include "connection.hpp"
#include "error.hpp"
#include "error_exception.hpp"
/*!
 * db namespace contains all classes which supports usage of database access
 * class generated by the dbgen2 code generator.
 * It basically supports:
 * - db::connection      - Establishment of connection to the database
 * - db::statement       - Support for statement execution
 * - db::error           - container for error description(s) (0 to many)
 * - db::error_dscr      - one database error description
 * - db::error_exception - exception which is raised whenever dbgen2 runtime
 *                         unhandled exception occurs
 */
namespace db
{
  /// environment handle
  std::int32_t connection::henv_ // NOLINT cppcoreguidelines-avoid-non-const-global-variables
    = 0;
  /// number of connections referencing the same environment handle
  std::atomic_uint
    connection::henv_ref_cnt_ // NOLINT cppcoreguidelines-avoid-non-const-global-variables
    = 0;

  /// lock mutex
  std::mutex mtx; // NOLINT -cppcoreguidelines-avoid-non-const-global-variables

  int connection::allocate_env_handle()
  {
    std::lock_guard<std::mutex> lck(mtx);
    int                         env_handle = 0;
    /* allocate an environment handle */
    auto rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env_handle);
    throw_on_error(rc,
                   env_handle,
                   SQL_HANDLE_ENV, //
                   "ERROR while allocating the environment handle.");
    return env_handle;
  }
  /*!
   * Constructor
   *
   * The constructor works in dual mode:
   * - as normal CLI connection
   *   one can:
   *   - provide only a database name
   *     - user name and password of currently logged user is assumed
   *   - provide database name, user and password
   * - using existing connection
   *   - it must be called after establishing of the connection
   *   - database name must be an empty string
   *
   * There is a memory leak in normal connect (20744 bytes) and 386,464 bytes,
   * if the connection process is interrupted by an exception.
   * Go IBM go.
   *
   * @param a_database a database to connect to
   * @param a_user a user which connects
   * @param a_password password of the user
   * @param a_log address of a log function (void log(std::string a_msg))
   * @param a_show_log whether to display log information immediately
   *                   (alternative is to use show_log method).
   *
   *                   One should pick a definition of an appropriate environment
   *                   variable, in order to be able to control the logging
   *                   process in runtime (e.g. getenv("logging"))
   * @exception error_exception
   *
   */
  connection::connection(cstr_t  a_database, // NOLINT bugprone-easily-swappable-parameters
                         cstr_t  a_user,     // NOLINT bugprone-easily-swappable-parameters
                         cstr_t  a_password, // NOLINT bugprone-easily-swappable-parameters
                         err_log a_log)
  : log_(a_log != nullptr ? a_log : &_log_)
  , db_name_(a_database)
  , user_(a_user)
  , pass_(a_password)
  , db_handle_(open_connection(db_name_, user_, pass_, log_))
  { }
  /*!
   * Within destructor connection to the database is closed and both handles (i.e. connection and
   * environment) are released. if statement cache is enabled, the cache is released also. \note
   * error codes are not tested on purpose.
   */
  connection::~connection() { close_connection(get_db_handle(), log_); }
  /**
   * @brief Method opens new database connection
   *
   * @param a_database name of the database
   * @param a_user username to connect to the database (can be empty)
   * @param a_password password (can be empty)
   * @return
   */
  int connection::open_connection(const std::string& a_database,
                                  const std::string& a_user,
                                  const std::string& a_password,
                                  err_log            log)
  {
    std::int16_t rc        = SQL_SUCCESS;
    int          db_handle = 0;
    try
    {
      /* allocate (or throw) an environment handle */
      if (henv_ref_cnt_ == 0) db::connection::henv_ = allocate_env_handle();
      /* set attribute to enable application to run as ODBC 3.0 application */
      rc = SQLSetEnvAttr(henv_,
                         SQL_ATTR_ODBC_VERSION,
                         reinterpret_cast<void*>(SQL_OV_ODBC3_80), // NOLINT
                         0);
      throw_on_error(rc, henv_, SQL_HANDLE_ENV, "ERROR enabling ODBC 3.8.");
      log("ODBC 3.8 enabled.");
      /* allocate a database connection handle */
      rc = SQLAllocHandle(SQL_HANDLE_DBC, henv_, &db_handle);
      throw_on_error(rc, henv_, SQL_HANDLE_ENV, "ERROR allocating db handle.");
      henv_ref_cnt_++; // here we are sure that the connection was successfully established
                       // and we can increase ref count for the environment handle
      log("DB handle allocated. #ref:" + std::to_string(henv_ref_cnt_));
      /* connect to the database */
      if (! a_database.empty()) [[unlikely]]
      { /* normal connection */
        rc = SQLConnect(db_handle,
                        reinterpret_cast<SQLCHAR*>(const_cast<char*>(a_database.data())), // NOLINT
                        SQL_NTS,
                        reinterpret_cast<SQLCHAR*>(const_cast<char*>(a_user.data())), // NOLINT
                        SQL_NTS,
                        reinterpret_cast<SQLCHAR*>(const_cast<char*>(a_password.data())), // NOLINT
                        SQL_NTS);
        throw_on_error(rc, db_handle, SQL_HANDLE_DBC, "ERROR connecting to the database.");
      }
      else { /* connection via previously established ESQL connection */
        rc = SQLConnect(db_handle, nullptr, SQL_NTS, nullptr, SQL_NTS, nullptr, SQL_NTS);
        throw_on_error(rc, db_handle, SQL_HANDLE_DBC, "ERROR connecting to the database.");
        log("DB connected. NULL connection.");
      }
      log(                                                     //
        std::string("DB connected. database:'") + a_database + //
        "' user:'" + a_user +                                  //
        "' handle: " + std::to_string(db_handle) +             //
        " #connections: " + std::to_string(henv_ref_cnt_));
      /* set AUTOCOMMIT OFF */
      rc = SQLSetConnectAttr(db_handle,
                             SQL_ATTR_AUTOCOMMIT,
                             reinterpret_cast<SQLPOINTER>(SQL_AUTOCOMMIT_OFF), // NOLINT
                             SQL_NTS);
      throw_on_error(rc, db_handle, SQL_HANDLE_DBC, "ERROR cannot switch off auto commit.");
      log("Autocommit disabled.");
      log("connection established db handle: " + std::to_string(db_handle) +
          " related db connections: " + std::to_string(henv_ref_cnt_));
      return db_handle;
    }
    catch (const error_exception& e)
    {
      close_connection(db_handle, log);
      throw;
    }
    // no return since we are throwing
  }
  /**
   * @brief drop database connection
   *
   */
  int connection::close_connection(int db_handle, err_log log)
  {
    int16_t rc = SQL_SUCCESS;
    if (db_handle != 0)
    {
      rc = SQLDisconnect(db_handle); // disconnect from the database
//      throw_on_error(rc, db_handle, SQL_HANDLE_DBC, "ERROR disconnecting.");
      henv_ref_cnt_--;
      rc = SQLFreeHandle(SQL_HANDLE_DBC, db_handle);
//      throw_on_error(rc, db_handle, SQL_HANDLE_DBC, "ERROR handle dbc.");
      log("handle released: h type:" + std::to_string(SQL_HANDLE_DBC) +
          " handle: " + std::to_string(db_handle));
    }
    log("connection destructor - (number still open databases): " + std::to_string(henv_ref_cnt_));
    /* free the environment handle */
    // if (henv_ref_cnt_ == 0)
    // {
    rc = SQLFreeHandle(SQL_HANDLE_ENV, henv_);
//    throw_on_error(rc, henv_, SQL_HANDLE_ENV, "ERROR handle env.");
    log("db environment handle released.");
    // }
    return rc;
  }
  /*!
   * fetch environment handle
   */
  std::int32_t connection::get_env() { return henv_; }
  /*!
   * fetch connection handle
   */
  std::int32_t connection::get_db_handle() const { return db_handle_; }
  /// commit
  std::int16_t connection::commit() const
  {
    // m_tran_count--;
    auto sts = SQLEndTran(SQL_HANDLE_DBC, db_handle_, SQL_COMMIT);
    (sts == SQL_SUCCESS) ? log("commit transaction.") : log("commit failed.");
    return sts;
  }
  /// rollback
  std::int16_t connection::rollback() const
  {
    auto sts = SQLEndTran(SQL_HANDLE_DBC, db_handle_, SQL_ROLLBACK);
    (sts == SQL_SUCCESS) ? log("rollback transaction.") : log("rollback failed.");
    return sts;
  }

  const std::string& connection::get_db_name() const { return db_name_; }
  /**
   * @brief serialize instance attributes
   *
   * @param a_msg message to be prepended to the serialized contnets
   * @return std::string serialized contents of the instance
   */
  std::string connection::dump(const std::string& a_msg) const
  {
    std::stringstream s;
    s << a_msg << " dbenvh: " << static_cast<int>(henv_)
      << " henv ref cnt: " << static_cast<int>(henv_ref_cnt_) << " user: " << user_
      << " db: " << db_name_ << " dbh: " << static_cast<int>(db_handle_)
      << " log: " << (log_ != nullptr);
    return s.str();
  }

  /// print current error context (code, line, file)
  void connection::print_ctx(const std::string& msg,
                             int                err_code,
                             int                line,
                             const std::string& file)
  {
    std::cerr << "msg: '" << msg << "'\n"
              << " error code: " << err_code << " line: " << line << " file: '" << file << "'"
              << std::endl;
  }
  /*!
   * The method returns address of the logging function.
   * @return address of log callback function
   */
  err_log connection::get_log() const { return log_; }
  /// log setter
  void connection::set_log(err_log a_log) { this->log_ = a_log; }

  void _log_(const std::string& msg)
  {
    if (db::connection::should_we_log()) std::cerr << msg << std::endl;
  }
  /*!
   * The method (re)sets connection attribute.
   * @param an_attr identification of the attribute
   * @param a_val new attribute value
   * @return SQL_SUCCESS or error status code
   *
   * Possible attribute identifications are:
   * - SQL_ACCESS_MODE
   * - SQL_AUTOCOMMIT
   * - SQL_LOGIN_TIMEOUT
   * - SQL_OPT_TRACE
   * - SQL_OPT_TRACEFILE
   * - SQL_TRANSLATE_DLL
   * - SQL_TRANSLATE_OPTION
   * - SQL_TXN_ISOLATION
   * - SQL_CURRENT_QUALIFIER
   * - SQL_ODBC_CURSORS
   * - SQL_QUIET_MODE
   * - SQL_PACKET_SIZE
   *
   * For a detailed and up to date list of attributes and their semantic please refer to IBM
   * documentation.
   */
  std::int16_t connection::set_attribute(const int an_attr, const int a_val) const
  {
    return SQLSetConnectAttr(db_handle_,
                             an_attr,
                             reinterpret_cast<SQLPOINTER>(a_val), // NOLINT
                             SQL_NTS);
  }
  /*!
   * The method releases the statement handle. Upon error it throws exception.
   *
   * @param h statement handle
   * @return result code of the db operation, whether the statement is released or
   *         just reset
   */
  std::int16_t connection::dealloc_stmt_handle(std::int32_t h)
  {
    auto rc = SQLFreeHandle(SQL_HANDLE_STMT, h);
    throw_on_error(
      rc, h, SQL_HANDLE_STMT, "Can't free statement handle handle=" + std::to_string(h) + ".");
    // log("Statement handle released. stmt handle='" + std::to_string(h)+ "'");
    return rc;
  }
  /**
   * @brief throws on error or return handle, handle_type pair
   *
   * @param rc status code of the operation
   * @param a_handle handle that might be associated with an error
   * @param a_handle_type handle type that might be associated with an error
   * @param a_msg error message to be sent in case of the error
   * @return
   */
  int connection::throw_on_error(std::int16_t       rc,
                                 std::int32_t       a_handle,
                                 std::int16_t       a_handle_type,
                                 const std::string& a_msg)
  {
    if (rc != SQL_SUCCESS)
    { // huston we have problems
      error err;
      err.load(a_handle, a_handle_type); // one or more errors from the RDBMS
      throw error_exception(err.dump(a_msg));
    }
    return rc; // no error; return handle, handle type pair
  }
  /// fetch free statement handle
  int connection::alloc_stmt_handle() const { return alloc_stmt_handle(db_handle_, log_); }
  /*!
   * Fetch free statement handle. Upon erro it throws exception.
   */
  int connection::alloc_stmt_handle(int db_handle, err_log log)
  {
    int  stmt_handle;
    auto rc = SQLAllocHandle(SQL_HANDLE_STMT, db_handle, &stmt_handle);
    throw_on_error(rc, db_handle, SQL_HANDLE_STMT, "Can not allocate statement handle.");
    log("Statement handle '" + std::to_string(stmt_handle) + "' allocated.");
    return stmt_handle;
  }
  /*!
   * The method logs a message in case that the log method is provided and logging
   * is enabled.
   * @param msg message to be written into a log
   */
  void connection::log(const std::string& msg) const
  {
    if (should_we_log())
    {
      if (log_ != nullptr) log_(msg);
      else std::cerr << msg;
    }
    else {
      /* empty on purpose - we do not want to log */
    }
  }
} // namespace db
