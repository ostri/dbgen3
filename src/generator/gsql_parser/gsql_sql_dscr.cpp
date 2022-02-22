#include "gsql_sql_dscr.hpp"
#include "multi_line.hpp"

namespace dbgen3
{

  gsql_sql_dscr::gsql_sql_dscr(const RDBMS& a_db, cstr_t sql, cstr_t sql_prep)
    : db_type_(a_db)
    , ml_sql_(sql)
    , ml_sql_prep_(sql_prep)
  { }

  gsql_sql_dscr::gsql_sql_dscr(const RDBMS& a_db, multi_line sql, multi_line sql_prep)
    : db_type_(a_db)
    , ml_sql_(std::move(sql))
    , ml_sql_prep_(std::move(sql_prep))
  { }

  std::string gsql_sql_dscr::dump() const { return dump("", 0); }

  std::string gsql_sql_dscr::dump(uint offs) const { return dump("", offs); }

  std::string gsql_sql_dscr::dump(cstr_t a_msg) const { return dump(a_msg, 0); }

  std::string gsql_sql_dscr::dump(cstr_t a_msg, uint offs) const
  {
    std::string s;
    // clang-format off
    if (! a_msg.empty()) s += out::sl(a_msg, offs);
    s += out::sl("sql:", offs);
    s += out::sl("{", offs);
//    s += out::sl(fmt::format("  key      : {}", key()), offs);
    s += out::sl(fmt::format("  db_type  : {:<7} {}", ME::enum_name(db_type_), ME::enum_integer(db_type_)), offs);
//    s += out::sl(fmt::format("  phase    : {:<7} {}", ME::enum_name(phase_), ME::enum_integer(phase_)), offs);
    s += out::sl("  sql     :", ml_sql_.dump(offs), offs);
    s += out::sl("  sql_prep:  ", ml_sql_prep_.dump(offs), offs);
    s += out::sl("}", offs);
    // clang-format on
    return s;
  }

  uint8_t gsql_sql_dscr::key() const { return ME::enum_integer<RDBMS>(db_type_); }
  std::string    gsql_sql_dscr::sql() const { return static_cast<const std::string>(ml_sql_);}
  std::string    gsql_sql_dscr::sql_prep() const { return static_cast<const std::string>(ml_sql_prep_);}

  RDBMS          gsql_sql_dscr::db_type() const { return db_type_; }

//  PHASE          gsql_sql_dscr::phase() const { return phase_; }

}; // namespace dbgen3