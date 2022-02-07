#ifndef GSQL_SQL_SET_HPP
#define GSQL_SQL_SET_HPP

#include <map>
#include <string>
#include <vector>

#include "gsql_sql_dscr.hpp"
#include "cmdline_parameters.hpp"
#include "string_format.hpp"

namespace dbgen3
{
  enum class INS_OP : int
  { // generic == SQL (standard SQL)
    // specific == SQL with extended features characteristic for a selected RDBMS
    inserted = 0, // no SQLs yet, just insert
    ambigous = 1, // two sql statements for the active RDBMS (both generic or both actve RDBMS)
    specific = 2, // replacing old generic with new specific
    other    = 3, // sql statement is for RDBMS that not selcted
    skip     = 4, // new SQL is generic, but we already have old specific SQL
                  
  };
  class gsql_sql_set
  {
  public:
    explicit gsql_sql_set(const RDBMS& db_type)
    : db_type_(db_type){};
    ~gsql_sql_set()                   = default;
    gsql_sql_set(const gsql_sql_set&) = default;
    gsql_sql_set(gsql_sql_set&&)      = default;
    gsql_sql_set& operator=(const gsql_sql_set&) = default;
    gsql_sql_set& operator=(gsql_sql_set&&) = default;

    uint        size() const;
    std::string dump() const;
    std::string dump(uint offs) const;
    std::string dump(cstr_t a_msg, uint offs) const;

    INS_OP               insert(const gsql_sql_dscr& a_val);
    const gsql_sql_dscr* fetch(const PHASE& a_phase) const;
    RDBMS                db_type() const;
    std::string          fetch_sql(const PHASE& a_phase) const;
  private:
    using sql_map    = std::map<uint, gsql_sql_dscr>;
    sql_map sql_map_ = {}; //!< dictionary of sql statements index by (rdbms, phase)
    RDBMS   db_type_;      //!< rdbms type that we are generating code for
  };

}; // namespace dbgen3

#endif // GSQL_SQL_SET_HPP
