/*!
 * \file
 * \brief Implementation of db::error utility class
 *
 * It holds a class which is used for error detection
 */
#include "error.hpp"
//#include "trace.hpp"
#include <array>
#include <cassert>
#include <iostream>
#include <sqlcli1.h>
#include <sstream>
#include <stdexcept>
#include <string>
namespace db
{
  // /*!
  //  *
  //  */
  // error_dscr::error_dscr(int         code  = 0,
  //                        const char* state = "",
  //                        const char* msg   = "")
  // : m_code(code)
  // , m_state(state)
  // , m_msg(msg)
  // {
  // }
  /// copy ctor
  // error_dscr::error_dscr(const error_dscr& o)
  // : m_code(o.m_code)
  // , m_state(o.m_state)
  // , m_msg(o.m_msg)
  // {
  // }

  /*!
   *
   */
  // error::error() noexcept
  //: // m_err_vec()
  //, m_stmt()
  // {
  // } // /// copy ctor
  // error::error(const error& o)
  // : m_err_vec(o.m_err_vec)
  // //, m_stmt(o.m_stmt)
  // {
  // }

  // /*!
  //  * Error constructor for statement usage
  //  * \
  //  */
  // error::error(const statement* a_stmt)
  // : m_err_vec()
  // // , m_stmt(a_stmt)
  // {
  //   load(a_stmt);
  // }
  /*!
   * Error constructor for handle usage
   * \param a_handle      - handle about which we want to obtain an error description
   * \param a_handle_type - type of the handle want to obtain an error description
   *                            - SQL_HANDLE_ENV for environment handles
   *                            - SQL_HANDLE_DBC for connection handles
   *                            - SQL_HANDLE_STMT for statement handles
   * \param a_msg         - additional message to be associated with the error description
   */
  error::error(SQLHANDLE a_handle, SQLSMALLINT a_handle_type, const char* a_msg)
  //  : m_err_vec()
  // , m_stmt()
  {
    load(a_handle, a_handle_type, a_msg);
  }
  // /*!
  //  * Destructor
  //  */
  // error::~error()
  // { /*__trace();*/
  // }
  /*!
   * \returns number of errors
   */
  uint error::get_err_count() const { return m_err_vec.size(); }
  /*!
   * \returns true if there is an error condition
   */
  bool error::is_error() const { return ! m_err_vec.empty(); }
  /*!
   * \returns i-th erro code or 0 if no code exists
   */
  int error::get_error_code(const unsigned int ndx) const
  {
    int result = 0;
    if (m_err_vec.size() > ndx) { result = m_err_vec.at(ndx).get_code(); }
    return result;
  }
  // /*!
  //  * The method loads vector with error descriptions.
  //  * \param a_stmt statement about which we want to obtain info
  //  * \returns a number of error descriptions that were loaded
  //  */
  // int error::load(const statement* a_stmt)
  // {
  //   m_stmt = a_stmt;
  //   return load(m_stmt->get_statement(), SQL_HANDLE_STMT, m_stmt->get_sql());
  // }
  /*!
   * The method loads the vector m_err_vec with error descriptions related to the provided handle
   * \param a_handle      - handle about which we want to obtain an error description
   * \param a_handle_type - type of the handle want to obtain an error description
   *                            - SQL_HANDLE_ENV for environment handles
   *                            - SQL_HANDLE_DBC for connection handles
   *                            - SQL_HANDLE_STMT for statement handles
   * \param a_msg         - additional message to be associated with the error description
   * \returns a number of error descriptions that were loaded
   *
   * Normally db2 returns only one error message, but it can be several.
   */
  int error::load(SQLHANDLE          a_handle,      // NOLINT bugprone-easily-swappable-parameters
                  SQLSMALLINT        a_handle_type, // NOLINT bugprone-easily-swappable-parameters
                  const std::string& a_msg)
  {
    const int max_msg_len   = 1024;
    const int max_state_len = 5;

    std::string state(max_state_len + 1, 0);
    std::string msg(max_msg_len + 1, 0);

    int16_t    msg_len   = 0;
    SQLINTEGER code      = 0;
    int        rc        = 0;
    int16_t    record_no = 1; /// records are counted from 1

    assert((a_handle_type == SQL_HANDLE_ENV) |  // NOLINT hicpp-no-array-decay
           (a_handle_type == SQL_HANDLE_DBC) |  // NOLINT hicpp-no-array-decay
           (a_handle_type == SQL_HANDLE_STMT)); // NOLINT hicpp-no-array-decay
    // clear possible previous error descriptions
    if (! m_err_vec.empty()) m_err_vec.clear();
    rc = SQLGetDiagRec(
      a_handle_type,
      a_handle,
      record_no,
      reinterpret_cast<SQLCHAR*>(&state[0]), // NOLINT cppcoreguidelines-pro-type-reinterpret-cast
      &code,
      reinterpret_cast<SQLCHAR*>(&msg[0]), // NOLINT cppcoreguidelines-pro-type-reinterpret-cast
      max_msg_len,
      &msg_len);
    bool end = false; // NOLINT altera-id-dependent-backward-branch
    while (! end)     // NOLINT
    {
      switch (rc)
      {
      case SQL_SUCCESS:
      {
        // state[max_state_len] = 0;
        // msg[msg_len]         = 0;
        std::string concat_msg("Err msg:'");
        concat_msg += std::string(msg.data(), msg_len) + std::string("'\n ") + std::string(a_msg);
        error_dscr err(code, state, concat_msg);
        m_err_vec.push_back(err);
        record_no++;

        rc = SQLGetDiagRec(
          a_handle_type,
          a_handle,
          record_no,
          reinterpret_cast<SQLCHAR*> // NOLINT cppcoreguidelines-pro-type-reinterpret-cast
          (&state[0]),
          &code,
          reinterpret_cast<SQLCHAR*>(&msg[0]), // NOLINT cppcoreguidelines-pro-type-reinterpret-cast
          max_msg_len,
          &msg_len);
        break;
      }
      case SQL_SUCCESS_WITH_INFO:
      {
        log("buffer for error description is to short. we need " + std::to_string(msg_len));
        break;
      }
      case SQL_INVALID_HANDLE:
      {
        log("Invalid handle " + std::to_string(a_handle));
        break;
      }
      case SQL_ERROR:
      {
        if (record_no < 1) { log("record number is less than 1. " + std::to_string(record_no)); }
        else if (max_msg_len < 0)
          log("message buffer length provided is less than 0. " + std::to_string(max_msg_len));
        else log("The asynchronous operation on the handle is not completed.");
        break;
      }
      case SQL_NO_DATA:
      {
        log("No more diagnostic records");
        break;
      }
      default:
      {
        log("Unknown error " + std::to_string(rc));
        break;
      }
      };
      end = rc != SQL_SUCCESS;
    }

    log(std::string(static_cast<const char*>(__func__)) + ": rc=" + std::to_string(rc) +
        " cnt:" + std::to_string(record_no));
    return (record_no - 1);
  }
  /// It fetches the number of error descriptions
  /// \returns number of error messages
  uint error::get_number_of_messages() const { return static_cast<int>(m_err_vec.size()); }

  // /*!
  //  * It assigns a new value to error description
  //  */

  // error_dscr& error_dscr::operator=(const error_dscr& o)
  // {
  //   if (&o != this)
  //   {
  //     //	std::cerr << "error_dscr::operator=" << std::endl;
  //     m_code  = o.m_code;
  //     m_state = o.m_state;
  //     m_msg   = o.m_msg;
  //   }
  //   return *this;
  // }

  /*!
   * It dumps the class instance to the stream
   */
  std::ostream& operator<<( // NOLINT
    std::ostream&     s,
    const error_dscr& o)
  {
    s << " state " << o.get_state() << " code  " << o.get_code() << " msg " << o.get_msg();
    return s;
  }

  /*!
   * It dumps the class instance to the stream
   */
  std::ostream& operator<<( // NOLINT
    std::ostream& s,
    const error&  o)
  {
    // // const statement* stmt = o.get_statement();
    // if (stmt) s << " sql:'" << stmt->get_sql() << "'\n";
    s << " #errors:'" << o.m_err_vec.size() << "'\n";
#pragma unroll 2
    for (unsigned int cnt = 0; cnt < o.m_err_vec.size(); cnt++)
    {
      s << " err [" << cnt << "]: " << o.m_err_vec.at(cnt) << std::endl;
    }
    return s;
  }

  // /// fetch a corresponding statement
  // const statement* error::get_statement() const { return m_stmt; }

  error_dscr error::get_error_dscr(const unsigned int ndx) const { return m_err_vec.at(ndx); }
  /*!
   * The method dumps all error descriptions in one string an returns it to the caller
   */
  std::string error::dump_errors() const
  {
    std::string        str;
    std::ostringstream s;

    s << " dump :" << static_cast<const error&>(*this);
    return s.str();
  }
  // ///Assignment operator
  // error& error::operator=(const error& o)
  // {
  //   assign(o);
  //   return *this;
  // }
  ///assign all members
  void error::assign(const error& o)
  {
    this->m_err_vec = o.m_err_vec;
    // this->m_stmt    = o.m_stmt;
  }
  void error::log(const std::string& /*msg*/) const
  { /*__t_log(msg);*/
  }

} // namespace db
