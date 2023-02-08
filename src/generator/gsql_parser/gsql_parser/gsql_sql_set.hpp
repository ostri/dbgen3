#ifndef GSQL_SQL_SET_HPP
#define GSQL_SQL_SET_HPP

#include <map>
#include <string>
#include <vector>

#include "gsql_sql_dscr.hpp"
#include "cmd_param/cmdline_parameters.hpp"
#include "utility_classes/string_format.hpp"

namespace dbgen3
{
  enum class INS_OP : int
  { // generic == SQL (standard SQL)
    // specific == SQL with extended features characteristic for a selected RDBMS
    inserted  = 0, // no SQLs yet, just insert
    ambiguous = 1, // two sql statements for the active RDBMS (both generic or both actve RDBMS)
    specific  = 2, // replacing old generic with new specific
    skip      = 3, // new SQL is generic, but we already have old specific SQL
  };
  class gsql_sql_set
  {
  public:
    explicit gsql_sql_set(const RDBMS& db_type)
    : db_type_(db_type){};
    ~gsql_sql_set()                              = default;
    gsql_sql_set(const gsql_sql_set&)            = default;
    gsql_sql_set(gsql_sql_set&&)                 = default;
    gsql_sql_set& operator=(const gsql_sql_set&) = default;
    gsql_sql_set& operator=(gsql_sql_set&&)      = default;

    std::string dump() const;
    std::string dump(uint offs) const;
    std::string dump(cstr_t a_msg, uint offs) const;

    INS_OP               put(const gsql_sql_dscr& a_val);
    const gsql_sql_dscr& get() const { return sql_dscr_; };
    RDBMS                db_type() const;
    std::string          sql() const;
    std::string          prep_sql() const;
    const multi_line&    sql_ml() const;
    const multi_line&    prep_sql_ml() const;
    bool                 is_empty() const { return this->empty_; }
  private:
    gsql_sql_dscr sql_dscr_{};           //!< sql descriptor (not owner)
    RDBMS         db_type_ = RDBMS::sql; //!< rdbms type that we are generating code for
    bool          empty_   = true;       //!< is the sql_dscr_ structure already loaded
  };

}; // namespace dbgen3

#endif // GSQL_SQL_SET_HPP
