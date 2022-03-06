#include "db2_reader.hpp"

namespace dbgen3
{

  int db2_reader::connect(cstr_t a_db_name, cstr_t a_user, cstr_t a_pass)
  {
    con_ = std::make_unique<db::connection>(std::string(a_db_name), std::string(a_user), std::string(a_pass));
    return 0;
  }

  const db::connection* db2_reader::connection() const { return con_.get(); }

};