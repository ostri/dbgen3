#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__
#include <stdexcept>

#include "program_status.hpp"

namespace dbgen3
{
  class dbgen3_exc : public std::runtime_error
  {
  public:
    dbgen3_exc(p_sts a_status, const std::string& a_msg)
    : std::runtime_error(a_msg), status_(a_status){}
    virtual ~dbgen3_exc()           = default;
    dbgen3_exc(const dbgen3_exc& o) = default;
    dbgen3_exc(dbgen3_exc&& o)      = default;
    dbgen3_exc& operator=(const dbgen3_exc&) = default;
    dbgen3_exc& operator=(dbgen3_exc&&) = default;
    p_sts g_status()const {return status_;}
  private:
    p_sts status_ = p_sts::unk_exception; //<! error code
  };

  class gsql_file_not_exsts : public dbgen3_exc
  {
  public:
    gsql_file_not_exsts(const std::string& a_msg)
    : dbgen3_exc(p_sts::gsql_file_not_exists, a_msg){};
  };
};     // namespace dbgen3
#endif // __EXCEPTIONS_H__