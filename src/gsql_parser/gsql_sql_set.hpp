#ifndef GSQL_SQL_SET_HPP
#define GSQL_SQL_SET_HPP

#include <vector>
#include <map>
#include <string>

#include "gsql_sql_dscr.hpp"
#include "cmdline_parameters.hpp"
#include "string_format.hpp"

namespace dbgen3
{
  class gsql_sql_set
  {
    using sql_set = std::vector<gsql_sql_dscr>;
    using sql_map = std::map<uint16_t, uint16_t>;
  public:
    gsql_sql_set()                    = default;
    ~gsql_sql_set()                   = default;
    gsql_sql_set(const gsql_sql_set&) = default;
    gsql_sql_set(gsql_sql_set&&)      = default;
    gsql_sql_set& operator=(const gsql_sql_set&) = default;

    uint        size() const { return sql_set_.size(); }
    std::string dump() const;
    std::string dump(uint offs) const;
    std::string dump(cstr_t a_msg, uint offs) const;

    bool                 insert(const gsql_sql_dscr a_sql);
    const gsql_sql_dscr fetch(const RDBMS& a_db, const PHASE& a_phase) const
    {
      auto key = gsql_sql_dscr::make_key(a_db, a_phase);
      auto it  = sql_map_.find(key);
      if (it != sql_map_.end()) { return (sql_set_[it->second]); }
      else return {};
    }
    const gsql_sql_dscr smart_fetch(const RDBMS& a_db, const PHASE& a_phase) const
    {
      auto r = fetch(a_db, a_phase);
      /// if not found for specific database, try to fetch definition fo rgeneric database
      if (r.key() == gsql_sql_dscr().key()) r = fetch(RDBMS::sql, a_phase);
      return r;
    }
    const std::string smart_fetch_sql(const RDBMS& a_db, const PHASE& a_phase) const
    {
      auto r = fetch(a_db, a_phase);
      /// if not found for specific database, try to fetch definition fo rgeneric database
      if (r.key() == gsql_sql_dscr().key()) r = fetch(RDBMS::sql, a_phase);
      return smart_fetch(a_db, a_phase).sql();
    }

  private:
    sql_set sql_set_; //!< vector of sql statement descriptions
    sql_map sql_map_; //!< ditionary of sql statements index by (rdbms, phase)
  };

  inline std::string gsql_sql_set::dump() const { return dump("", 0); };

  inline std::string gsql_sql_set::dump(uint offs) const { return dump("", offs); };

  inline std::string gsql_sql_set::dump(cstr_t a_msg, uint offs) const
  {
    std::string s;
    if (! a_msg.empty()) s += out::sl(a_msg, offs);
    s += out::sl("sql_set:", offs);
    s += out::sl("{", offs);
    for (auto const& [key, val] : sql_map_) { s += sql_set_[val].dump(offs + 2); }
    s += out::sl("}", offs);
    return s;
  };
}; // namespace dbgen3

#endif // GSQL_SQL_SET_HPP
