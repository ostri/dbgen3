/*!
 * \file
 * \brief Implementation of db::error utility class
 *
 * It holds a class which is used for error detection
 */
#include "error.hpp"
#include "error_dscr.hpp"
//#include "trace.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <sqlcli1.h>
#include <sstream>
#include <stdexcept>
#include <string>
namespace db
{
  /*!
   * Error constructor for handle usage
   * \param a_handle      - handle about which we want to obtain an error description
   * \param a_handle_type - type of the handle want to obtain an error description
   *                            - SQL_HANDLE_ENV for environment handles
   *                            - SQL_HANDLE_DBC for connection handles
   *                            - SQL_HANDLE_STMT for statement handles
   * \param a_msg         - additional message to be associated with the error description
   */
  error::error(SQLHANDLE a_handle, SQLSMALLINT a_handle_type) { load(a_handle, a_handle_type); }
  /*!
   * \returns number of errors
   */
  uint error::get_err_count() const { return errors_.size(); }
  /*!
   * \returns true if there is an error condition
   */
  bool error::is_error() const { return ! errors_.empty(); }

  // NOLINTNEXTLINE bugprone-easily-swappable-parameters
  int16_t error::wrap_diag_rec(int32_t a_handle, int16_t a_handle_type)
  {
    int16_t   rc;
    str_t     e_msg;
    const int max_msg_len   = 1024;
    const int max_state_len = 5;

    std::array<char, max_state_len + 1> state{};
    std::array<char, max_msg_len + 1>   msg{};
    int16_t                             msg_len = 0;
    int32_t                             code    = 0;

    rc = SQLGetDiagRec(a_handle_type,
                       a_handle,
                       static_cast<int16_t>(errors_.size() + 1),
                       // NOLINTNEXTLINE cppcoreguidelines-pro-type-reinterpret-cast
                       reinterpret_cast<SQLCHAR*>(state.data()),
                       &code,
                       // NOLINTNEXTLINE cppcoreguidelines-pro-type-reinterpret-cast
                       reinterpret_cast<SQLCHAR*>(msg.data()),
                       max_msg_len,
                       &msg_len);
    switch (rc)
    {
    case SQL_SUCCESS:
    {
      //      error_dscr err(code, cstr_t(state.data(), state.size()), cstr_t(msg.data(), msg_len));
      auto state_len = std::strlen(state.data());
      errors_.emplace_back(code, cstr_t(state.data(), state_len), cstr_t(msg.data(), msg_len));
      break;
    }
    case SQL_SUCCESS_WITH_INFO:
    {
      e_msg = "buffer for error description is to short. we need " + std::to_string(msg_len);
      log(e_msg);
      throw std::runtime_error(e_msg);
    }
    case SQL_INVALID_HANDLE:
    {
      e_msg = "Invalid handle " + std::to_string(a_handle);
      log(e_msg);
      throw std::runtime_error(e_msg);
    }
    case SQL_ERROR:
    {
      auto record_no = errors_.size() + 1;
      if (record_no < 1) /// not possible
        e_msg = "record number is less than 1. " + std::to_string(record_no);
      else if (max_msg_len < 0) /// not possible
        e_msg = "message buffer length provided is less than 0. " + std::to_string(max_msg_len);
      else /// not possible ; made in sychronous way
        e_msg = "The asynchronous operation on the handle is not completed.";
      log(e_msg);
      throw std::runtime_error(e_msg);
    }
    case SQL_NO_DATA:
    {
      // log("No more diagnostic records");
      break;
    }
    default:
    {
      e_msg = "Unknown error " + std::to_string(rc) + " file:" + __FILE__ + " " +
              std::to_string(__LINE__);
      log(e_msg);
      throw std::runtime_error(e_msg);
    }
    };
    return rc;
  }
  std::size_t error::load_stmt_err(SQLHANDLE a_handle) { return load(a_handle, SQL_HANDLE_STMT); }
  /*!
   * The method loads the vector m_err_vec with error descriptions related to the provided handle
   * \param a_handle      - handle about which we want to obtain an error description
   * \param a_handle_type - type of the handle want to obtain an error description
   *                            - SQL_HANDLE_ENV for environment handles
   *                            - SQL_HANDLE_DBC for connection handles
   *                            - SQL_HANDLE_STMT for statement handles
   * \returns a number of error descriptions that were loaded
   *
   * Normally db2 returns only one error message, but it can be several.
   */
  std::size_t error::load(SQLHANDLE a_handle, SQLSMALLINT a_handle_type)
  {
    assert((a_handle_type == SQL_HANDLE_ENV) |  // NOLINT hicpp-no-array-decay
           (a_handle_type == SQL_HANDLE_DBC) |  // NOLINT hicpp-no-array-decay
           (a_handle_type == SQL_HANDLE_STMT)); // NOLINT hicpp-no-array-decay
    // clear possible previous error descriptions (multiple loads on error object instance)
    errors_.clear();
    while (wrap_diag_rec(a_handle, a_handle_type) == SQL_SUCCESS) { }
    return errors_.size();
  }
  /*!
   * The method dumps all error descriptions in one string an returns it to the caller
   */
  std::string error::dump(cstr_t a_msg) const
  {
    std::string s(a_msg);
    for (const auto& e_dscr : errors_) { s += e_dscr.dump("") + "\n"; }
    return s;
  }
  void error::log(const std::string& msg) { std::cerr << msg; }

} // namespace db
