#include "error_exception.hpp"
/*!
 * \file
 * \brief Implementation of db::error_exception class
 */
#include "../runtime/error.hpp"
#include "../runtime/error_exception.hpp"
#include <iostream>
#include <stdexcept>

namespace db
{
  /// ctor
  error_exception::error_exception(const error& err)
  : std::runtime_error(err.dump_errors())
  , m_error(new error(err))
  {
  }
  /// ctor
  error_exception::error_exception(const std::string& str)
  : std::runtime_error(str)
  , m_error(
      new error) // create a default error object so that getter will not return null pointer
  {
  }
  /// copy ctor
  error_exception::error_exception(const error_exception& o)
  : std::runtime_error(o)
  , m_error(new error(*o.m_error))
  {
  }
  /// dtor
  error_exception::~error_exception() noexcept
  {
    if (m_error != nullptr)
    {
      delete m_error;
      m_error = nullptr; // ziher je ziher je rekla nuna in dala kondom na metlo
    }
  }
  const db::error& error_exception::get_error() const { return *m_error; }
  /*!
   * It dumps the class instance to the stream
   */

  std::ostream& operator<< // NOLINT fuchsia-overloaded-operator
    (std::ostream& s, const error_exception& o)
  {
    s << " error(s):" << o.what();
    return s;
  }
} // namespace db
