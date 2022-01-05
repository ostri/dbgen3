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
#include <string>
#include <vector>

#include "sqlcli1.h"

//#include "statement.hpp"
namespace db
{

  //  class statement;
  /*!
   * \brief It holds error description data
   *
   * \ingroup runtime
   *
   * Class error_dscr holds information about one error
   * description (there can be several)
   */
  class error_dscr
  {
    //friend class error;
  public:
    ///constructor
    error_dscr(int code, std::string state, std::string msg);
    /// copy constructor
    error_dscr(const error_dscr& o) = default;
    error_dscr(error_dscr&& o)      = default;
    error_dscr& operator=(const error_dscr& o) = default;
    error_dscr& operator=(error_dscr&& o) = default;
    /// destructor
    virtual ~error_dscr() = default;
    /// getters
    int                get_code() const { return m_code; }
    const std::string& get_state() const { return m_state; }
    const std::string& get_msg() const { return m_msg; }
  private:
    int         m_code = 0; //!< sql code
    std::string m_state;    //!< sql state
    std::string m_msg;      //!< error description
  };
  /// Serialisation to string stream
  std::ostream& operator<<(std::ostream& s, const error_dscr& o);


  /*!
   * \brief It holds information about errors of the associated statement
   *
   * Class error obtain and holds information about errors related
   * to a provided statement (handle).
   */
  class error
  {
    friend std::ostream& operator<<(std::ostream& s, const error& o);
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
     * @param a_msg         additinal message
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
    error_dscr get_error_dscr(uint ndx = 0) const;
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
    // // /// corresponding statement
    // const statement* m_stmt;
  };
  /// Serialisation to string stream
  std::ostream& operator<<(std::ostream& s, const error& o);
} // namespace db
#endif
