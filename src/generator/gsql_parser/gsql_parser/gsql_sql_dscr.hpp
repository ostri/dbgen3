#ifndef GSQL_SQL_DSCR_HPP
#define GSQL_SQL_DSCR_HPP

#include <fmt/core.h>
#include <vector>

#include "utility_classes/string_format.hpp"
#include "cmd_param/cmdline_parameters.hpp"
#include "utility_classes/multi_line.hpp"
namespace dbgen3
{
  class gsql_sql_dscr
  {
  public:
    gsql_sql_dscr() = default;
    gsql_sql_dscr(const RDBMS& a_db, cstr_t sql, cstr_t sql_prep);
    gsql_sql_dscr(const RDBMS& a_db, multi_line sql, multi_line sql_prep);

    ~gsql_sql_dscr()                    = default;
    gsql_sql_dscr(const gsql_sql_dscr&) = default;
    gsql_sql_dscr(gsql_sql_dscr&&)      = default;
    gsql_sql_dscr&    operator=(const gsql_sql_dscr&) = default;
    gsql_sql_dscr&    operator=(gsql_sql_dscr&&) = default;
    std::string       dump() const;
    std::string       dump(uint offs) const;
    std::string       dump(cstr_t a_msg) const;
    std::string       dump(cstr_t a_msg, uint offs) const;
    uint8_t           key() const;
    std::string       sql() const;
    std::string       sql_prep() const;
    const multi_line& sql_ml() const { return ml_sql_; }
    const multi_line& sql_prep_ml() const { return ml_sql_prep_; }
    RDBMS             db_type() const;
//    PHASE             phase() const;
  private:
    /*............................................................*/
    enum RDBMS db_type_ = RDBMS::sql;     //!< rdbms type (dialect)
    multi_line ml_sql_{};                 //!< sql statement optionaly across several lines
    multi_line ml_sql_prep_{};            //!< optional prepare sql
  };
}; // namespace dbgen3

#endif // GSQL_SQL_DSCR_HPP
