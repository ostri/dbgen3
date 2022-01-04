#ifndef ERROR_EXCEPTION_H
#define ERROR_EXCEPTION_H

/*!
 * \file
 * \brief Declaration of db::error_exception utility class
 *
 * It holds a class which is used for raising exceptions
 * Updates
 * When       Who What
 * 18.05.2009 Os3 Predecessor of error_exception became std::runtime_error
 */
#include "error.hpp"
#include <iostream>
#include <stdexcept>

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
    error_exception(const error& err);
    /// ctor
    error_exception(const std::string& str);
    /// copy ctor
    error_exception(const error_exception& o);
    /// dtor
    virtual ~error_exception() throw();
    /// gettter
    const error& get_error() const;

  private:
    /// assign operator
    error_exception& operator=(const error_exception& e);
    /// error data holder
    error* m_error;
  };
  /// Serialisation to string stream
  std::ostream& operator<<(std::ostream& s, const error_exception& o);
} // namespace db
#endif
