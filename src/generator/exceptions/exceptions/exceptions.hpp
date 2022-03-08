#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>

#include "utility_classes/program_status.hpp"

namespace dbgen3
{
  class dbgen3_exc : public std::runtime_error
  {
  public:
    dbgen3_exc(P_STS a_status, const std::string& a_msg);
    ~dbgen3_exc() override          = default;
    dbgen3_exc(const dbgen3_exc& o) = default;
    dbgen3_exc(dbgen3_exc&& o)      = default;
    dbgen3_exc& operator=(const dbgen3_exc&) = default;
    dbgen3_exc& operator=(dbgen3_exc&&) = default;
    P_STS       status() const;
  private:
    P_STS status_ = P_STS::unk_exception; //<! error code
  };

  class gsql_file_not_exsts : public dbgen3_exc
  {
  public:
    explicit gsql_file_not_exsts(const std::string& a_msg)
    : dbgen3_exc(P_STS::gsql_file_not_exists, a_msg){};
  };
};     // namespace dbgen3

#endif // EXCEPTIONS_HPP
