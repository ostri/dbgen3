#ifndef DB2_READER_HPP
#define DB2_READER_HPP

#include <string>

#include "common.hpp"
#include "connection.hpp"
#include "error_exception.hpp"

namespace dbgen3
{
  class db2_reader
  {
  public:
    db2_reader()=default;
    int connect(cstr_t a_db_name, cstr_t a_user, cstr_t a_pass)
    {
      con_ = std::make_unique<db::connection>(std::string(a_db_name), std::string(a_user), std::string(a_pass));
      return 0;
    }
    const db::connection* connection() const { return con_.get(); }
  protected:
  private:
    std::string           db_name_;
    std::string           user_;
    std::string           pass_;
    std::unique_ptr<const db::connection> con_; //!< database connection
  };
};     // namespace dbgen3
#endif // DB2_READER_HPP
