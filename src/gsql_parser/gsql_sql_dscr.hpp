#ifndef GSQL_SQL_HPP
#define GSQL_SQL_HPP

#include <vector>
#include <fmt/core.h>

#include "string_format.hpp"
#include "cmdline_parameters.hpp"
#include "multi_line.hpp"

namespace dbgen3
{
  class gsql_sql_dscr
  {
  public:
    gsql_sql_dscr() = default;
    gsql_sql_dscr(const RDBMS& a_db, const PHASE& a_phase, const multi_line& sql);
    gsql_sql_dscr(const RDBMS& a_db, const PHASE& a_phase, const std::string& sql);

    ~gsql_sql_dscr()                    = default;
//    ~gsql_sql_dscr() { err << dump(fmt::format("destructor: {}", fmt::ptr(this)), 0); }
    gsql_sql_dscr(const gsql_sql_dscr&) = default;
//    gsql_sql_dscr(const gsql_sql_dscr&) { err << dump(fmt::format("copy constructor: {}", fmt::ptr(this)), 0); }
    gsql_sql_dscr(gsql_sql_dscr&&)      = default;
    gsql_sql_dscr& operator=(const gsql_sql_dscr&) = default;
    gsql_sql_dscr& operator=(gsql_sql_dscr&&) = default;
    std::string    dump() const;
    std::string    dump(uint offs) const;
    std::string    dump(cstr_t a_msg) const;
    std::string    dump(cstr_t a_msg, uint offs) const;
    static uint    make_key(const RDBMS& a_db, const PHASE& a_phase);
    uint           key() const;
  private:
    /*............................................................*/
    enum RDBMS db_type_ = RDBMS::sql;     //!< rdbms type (dialect)
    enum PHASE phase_   = PHASE::invalid; //!< SQL statement phase
    multi_line ml_sql_;                   //!< sql statement optionaly across several lines
  };

  inline gsql_sql_dscr::gsql_sql_dscr(const RDBMS&      a_db,
                                      const PHASE&      a_phase,
                                      const multi_line& sql)
  : db_type_(a_db)
  , phase_(a_phase)
  , ml_sql_(sql)
  {
    // err << out::sl("multiline");
    // err << dump(fmt::format("constructor: {}", fmt::ptr(this)));
  }

  inline gsql_sql_dscr::gsql_sql_dscr(const RDBMS&       a_db,
                                      const PHASE&       a_phase,
                                      const std::string& sql)
  : db_type_(a_db)
  , phase_(a_phase)
  , ml_sql_(sql)
  {
    // // ml_sql_ = sql;
    // err << dump(fmt::format("constructor: {}", fmt::ptr(this)));
  }

  inline std::string gsql_sql_dscr::dump() const { return dump("", 0); }

  inline std::string gsql_sql_dscr::dump(uint offs) const { return dump("", offs); }

  inline std::string gsql_sql_dscr::dump(cstr_t a_msg) const { return dump(a_msg, 0); }

  inline std::string gsql_sql_dscr::dump(cstr_t a_msg, uint offs) const
  {
    std::string s;
    if (! a_msg.empty()) s += out::sl(a_msg, offs);
    s += out::sl("sql:", offs);
    s += out::sl("{", offs);
    s += out::sl("  key      :", std::to_string(key()), offs);
    s += out::sl("  db_type  :", ME::enum_name(db_type_), offs);
    s += out::sl("  phase    :", ME::enum_name(phase_), offs);
    s += out::sl("  statement:", ml_sql_.dump(offs), offs);
    s += out::sl("}", offs);
    return s;
  }

  inline uint gsql_sql_dscr::make_key(const RDBMS& a_db, const PHASE& a_phase)
  {
    return static_cast<uint>(a_db) * 10 + static_cast<uint>(a_phase);
  }

  inline uint gsql_sql_dscr::key() const { return make_key(db_type_, phase_); }
}; // namespace dbgen3

#endif // GSQL_SQL_HPP
