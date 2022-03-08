#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__
#include <stdexcept>

#include "exceptions/exceptions.hpp"
// #include "magic_enum.hpp"
// #include "utility_classes/program_status.hpp"

namespace dbgen3
{
  dbgen3_exc::dbgen3_exc(P_STS a_status, const std::string& a_msg)
  : std::runtime_error(a_msg)
  , status_(a_status)
  { }
  P_STS       dbgen3_exc::status() const { return status_; }
  // class dbgen3_exc : public std::runtime_error
  // {
  // public:
  //   dbgen3_exc(P_STS a_status, const std::string& a_msg)
  //   : std::runtime_error(a_msg)
  //   , status_(a_status)
  //   { }
  //   ~dbgen3_exc() override          = default;
  //   dbgen3_exc(const dbgen3_exc& o) = default;
  //   dbgen3_exc(dbgen3_exc&& o)      = default;
  //   dbgen3_exc& operator=(const dbgen3_exc&) = default;
  //   dbgen3_exc& operator=(dbgen3_exc&&) = default;
  //   P_STS       status() const { return status_; }
  //   uint        status_int() const { return ME::enum_integer<P_STS>(status_); }
  // private:
  //   P_STS status_ = P_STS::unk_exception; //<! error code
  // };

  // class gsql_file_not_exsts : public dbgen3_exc
  // {
  // public:
  //   explicit gsql_file_not_exsts(const std::string& a_msg)
  //   : dbgen3_exc(P_STS::gsql_file_not_exists, a_msg){};
  // };
};     // namespace dbgen3
#endif // __EXCEPTIONS_H__