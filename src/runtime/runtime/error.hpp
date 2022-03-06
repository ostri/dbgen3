/*!
 * \file
 * \brief Declaration of db::error utility class
 *
 * It holds a class which is used for error detection
 *
 * History of changes
 * 20.5.2009 Os3 added int load(SQLHANDLE a_handle, SQLSMALLINT a_handle_type, const char* a_msg);
 */
#ifndef ERROR_H
#define ERROR_H

//#include "error_exception.hpp"
#include <sstream>
#include <string>
#include <vector>

#include "sqlcli1.h"
#include "error_dscr.hpp"

//#include "statement.hpp"
namespace db
{
  using cstr_t = std::string_view;
  /*!
   * \brief It holds information about errors of the associated statement
   *
   * Class error obtain and holds information about errors related
   * to a provided statement (handle).
   */
  class error
  {
    //    friend std::ostream& operator<<(std::ostream& s, const error& o);
  public:
    /// constructor
    error() = default;
    /// destructor
    virtual ~error() = default;
    /// copy constructor
    error(const error& o) = default;
    /// move ctor
    error(error&& o) = default;
    /// Assignment operator
    error& operator=(const error& o) = default;
    /// move assignement operator
    error& operator=(error&& o) = default;
    /// handle constructor
    error(SQLHANDLE a_handle, SQLSMALLINT a_handle_type);
    /**
     * @brief loads error description from the provided handle
     *
     * @param a_handle      handle with error descriptions
     * @param a_handle_type type of the handle
     * @return number of the errors loaded
     */
    // NOLINTNEXTLINE bugprone-easily-swappable-parameters
    std::size_t load(SQLHANDLE a_handle, SQLSMALLINT a_handle_type);
    std::size_t load_stmt_err(SQLHANDLE a_handle); //!< load for statements only
    /// getters
    /// is there some error
    bool is_error() const;
    /// it writes all errors into a string
    std::string dump(cstr_t a_msg = "") const;
    /// fetch a corresponding error code
    int get_error_code(uint ndx) const; // FIXME handle_return_code
    const std::vector<error_dscr>& errors() const { return this->errors_; }
  private:
    /// fetch the number of error descriptions
    uint get_number_of_messages() const;
    uint get_err_count() const;
    /// fetch n-th error description from the list of descriptions
    const error_dscr& get_error_dscr(uint ndx = 0) const;
    /// log - for statements only
    static void log(const std::string& msg);
    int16_t     wrap_diag_rec(int32_t a_handle, int16_t a_handle_type);
    /*........................................................................*/
    /// vector of errors
    std::vector<error_dscr> errors_ = {};
  };
  /// fetch nth error description
  inline const error_dscr& error::get_error_dscr(uint ndx) const { return errors_.at(ndx); }
  /*!
   * \returns i-th erro code or 0 if no code exists
   */
  inline int error::get_error_code(uint ndx) const { return errors_.at(ndx).get_code(); }
  /// Fetch the number of error descriptions
  inline uint error::get_number_of_messages() const { return errors_.size(); }
} // namespace db
#endif
