#ifndef GSQL_SQL_HPP
#define GSQL_SQL_HPP

#include <vector>

#include "string_format.hpp"
#include "cmdline_parameters.hpp"
#include "multi_line.hpp"

namespace dbgen3
{
  // /// list of supported language generators
  // enum class phase : uint
  // {
  //   invalid = 0, //!< placeholder for all invalid inputs
  //   prepare = 1, //!< SQL to prepare environment for generator
  //   main    = 2, //!< SQL that is actually executed at runtime
  //   cleanup = 3, //!< SQL to teardown the environment for generator
  // };


  class gsql_sql
  {
  public:
    gsql_sql() = default;
    gsql_sql(RDBMS& a_db, PHASE& a_phase, const multi_line& sql)
    : db_type_(a_db)
    , phase_(a_phase)
    , ml_sql_(sql)
    { }
    gsql_sql(RDBMS& a_db, PHASE& a_phase, cstr_t sql)
    : db_type_(a_db)
    , phase_(a_phase)
    , ml_sql_(sql)
    { }

    ~gsql_sql()               = default;
    gsql_sql(const gsql_sql&) = default;
    gsql_sql(gsql_sql&&)      = default;
    gsql_sql& operator=(const gsql_sql&) = default;
    gsql_sql& operator=(gsql_sql&&) = default;
    // auto operator<=>(const gsql_sql&) const = default;
    // bool        operator==(const gsql_sql& o) { return this->key() == o.key(); }
    std::string dump() const { return dump("", 0); }
    std::string dump(uint offs) const { return dump("", offs); }
    std::string dump(cstr_t a_msg, uint offs) const
    {
      std::string s;
      if (! a_msg.empty()) s += out::sl(a_msg, offs);
      s += out::sl("sql:", offs);
      s += out::sl("{", offs);
      s += out::sl("  key:", std::to_string(key()), offs);
      s += out::sl("  db_type:", std::to_string(static_cast<uint>(db_type_)), offs);
      s += out::sl("  phase:", std::to_string(static_cast<uint>(phase_)), offs);
      s += out::sl("  cmd:", ml_sql_.dump(offs), offs);
      s += out::sl("}", offs);
      return s;
    }
    static uint make_key(const RDBMS& a_db, const PHASE& a_phase);
    uint        key() const { return make_key(db_type_, phase_); }
  private:
    /*............................................................*/
    enum RDBMS db_type_ = RDBMS::sql;   //!< rdbms type (dialect)
    enum PHASE phase_   = PHASE::invalid; //!< SQL statement phase
    multi_line ml_sql_;                   //!< sql statement optionaly accros several lines
  };

  inline uint gsql_sql::make_key(const RDBMS& a_db, const PHASE& a_phase)
  {
    return static_cast<uint>(a_db) * 1000 + static_cast<uint>(a_phase);
  }
}; // namespace dbgen3

#endif // GSQL_SQL_HPP
