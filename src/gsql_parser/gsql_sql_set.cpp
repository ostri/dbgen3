#include "gsql_sql_set.hpp"

namespace dbgen3
{
  bool gsql_sql_set::insert(const gsql_sql_dscr a_sql)
  {
    gsql_sql_dscr sql(a_sql);
    sql_set_.push_back(sql);
    const auto [it, sts] = sql_map_.emplace(sql.key(), sql_set_.size() - 1);
    if (! sts) sql_set_.pop_back(); // if map fails, we need to restore consistent state
    return sts;
  }

  const gsql_sql_dscr gsql_sql_set::fetch(const RDBMS& a_db, const PHASE& a_phase) const
  {
    auto key = gsql_sql_dscr::make_key(a_db, a_phase);
    auto it  = sql_map_.find(key);
    if (it != sql_map_.end()) { return sql_set_[it->second]; }
    else return{};
  }

  const gsql_sql_dscr gsql_sql_set::smart_fetch(const RDBMS& a_db, const PHASE& a_phase) const
  {
    auto r = fetch(a_db, a_phase);
    if (r.key() == gsql_sql_dscr().key()) r = fetch(RDBMS::db2, a_phase);
    return r;
  }
} // namespace dbgen3