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
    ///Assignment operator
    error& operator=(const error& o) = default;
    /// move assignement operator
    error& operator=(error&& o) = default;
    /// handle constructor
    error(SQLHANDLE a_handle, SQLSMALLINT a_handle_type, const std::string& a_msg);
    /**
     * @brief loads error description from the provided handle
     *
     * @param a_handle      handle with error descriptions
     * @param a_handle_type type of the handle
     * @return number of the errors loaded
     */
    int load(SQLHANDLE a_handle, SQLSMALLINT a_handle_type, const std::string& a_msg);
    /// getters
    uint get_err_count() const;
    /// is there some error
    bool is_error() const;
    /// fetch a corresponding error code
    int get_error_code(uint ndx = 0) const;
    /// fetch n-th error description from the list of descriptions
    const error_dscr& get_error_dscr(uint ndx = 0) const;
    /// fetch the number of error descriptions
    uint get_number_of_messages() const;
    /// it writes all errors into a string
    std::string dump() const;
  private:
    /// log - for statements only
    static void log(const std::string& msg);
    /*........................................................................*/
    /// vector of errors
    std::vector<error_dscr> errors_ = {};
  };
  /// fetch nth error description
  inline const error_dscr& error::get_error_dscr(uint ndx) const { return errors_.at(ndx); }
  /// Fetch the number of error descriptions
  inline uint error::get_number_of_messages() const { return errors_.size(); }
} // namespace db
#endif
