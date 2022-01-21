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

} // namespace dbgen3