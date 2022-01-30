#ifndef GSQL_SQL_DSCR_HPP
#define GSQL_SQL_DSCR_HPP

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
    gsql_sql_dscr(const gsql_sql_dscr&) = default;
    gsql_sql_dscr(gsql_sql_dscr&&)      = default;
    gsql_sql_dscr& operator=(const gsql_sql_dscr&) = default;
    gsql_sql_dscr& operator=(gsql_sql_dscr&&) = default;
    std::string    dump() const;
    std::string    dump(uint offs) const;
    std::string    dump(cstr_t a_msg) const;
    std::string    dump(cstr_t a_msg, uint offs) const;
    uint8_t        key() const;
    std::string    sql() const;
    RDBMS          db_type() const { return db_type_; }
    PHASE          phase() const { return phase_; }
  private:
    /*............................................................*/
    enum RDBMS db_type_ = RDBMS::sql;     //!< rdbms type (dialect)
    enum PHASE phase_   = PHASE::invalid; //!< SQL statement phase
    multi_line ml_sql_;                   //!< sql statement optionaly across several lines
  };
}; // namespace dbgen3

#endif // GSQL_SQL_DSCR_HPP
