/*!
 * \file
 * \brief Declaration of db::error_exception utility class
 *
 * It holds a class which is used for raising exceptions
 * Updates
 * When       Who What
 * 18.05.2009 Os3 Predecessor of error_exception became std::runtime_error
 */
#ifndef ERROR_EXCEPTION_H
#define ERROR_EXCEPTION_H

#include <iostream>
#include <stdexcept>

#include "error.hpp"

namespace db
{
  /*!
   * \brief runtime exception
   *
   * The class holds all relevant data needed to signal the raised exception. All exceptions, which are raised by
   * dbgen2 runtime, are instances of runtime_exception or its offspring.
   *
   * \ingroup runtime
   *
   */
  class error_exception : public std::runtime_error
  {
  public:
    /// ctor
    explicit error_exception(const error& err) noexcept;
    /// ctor
    explicit error_exception(const std::string& str) noexcept;
    /// copy ctor
    error_exception(const error_exception& o) noexcept;
    /// move constructor
    error_exception(error_exception&& o) noexcept = default;
    error_exception& operator=(const error_exception& o) = default;
    error_exception& operator=(error_exception&& o) = default;
    /// dtor
    ~error_exception() override = default;
    /// gettter
    // const error& get_error() const;
  private:
    // /// error data holder
    // const error& m_error;
  };
  /// Serialisation to string stream
  std::ostream& operator<<(std::ostream& s, const error_exception& o);
  // /*!
  //  * It dumps the class instance to the stream
  //  */
  // inline std::ostream& operator<< // NOLINT fuchsia-overloaded-operator
  //   (std::ostream& s, const error_exception& o)
  // {
  //   s << " error(s):'" << o.what() << "'.";
  //   return s;
  // }

  /// ctor
  inline error_exception::error_exception(const error& err) noexcept
  : std::runtime_error(err.dump())
  {
  }

  /// ctor
  inline error_exception::error_exception(const std::string& str) noexcept
  : std::runtime_error(str)
  {
  }
} // namespace db
#endif
