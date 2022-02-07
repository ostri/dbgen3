#include "gsql_sql_set.hpp"

namespace dbgen3
{
  uint gsql_sql_set::size() const { return sql_map_.size(); }

  std::string gsql_sql_set::dump() const { return dump("", 0); };

  std::string gsql_sql_set::dump(uint offs) const { return dump("", offs); };

  std::string gsql_sql_set::dump(cstr_t a_msg, uint offs) const
  {
    std::string s;
    if (! a_msg.empty()) s += out::sl(a_msg, offs);
    s += out::sl("sql_set:", offs);
    s += out::sl("{", offs);
    for (auto const& [key, val] : sql_map_) { s += val.dump(offs + 2); }
    s += out::sl("}", offs);
    return s;
  };
  /**
   * @brief insert the sql descriptor to the structure
   *
   * The metod tries to insert the SQL descriptior into the structure
   * - new descriptior is generic or for specific database equal to curren trdbms
   *   yes
   *     there is old descriptor for specific phase
   *     yes  specific == db2, oracle, ..., generic == sql
   *       (old specific, new specific) : ambiguity              ==
   *       (old specific, new generic)  : skip new one           >
   *       (old generic,  new specific) : replace old generic    <
   *       (old generic,  new generic)  : ambiguity              ==
   *     no insert the descriptor true
   *   no - skip it - false
   *
   * @param a_sql_dscr sql descriptor
   * @return 0 - inserted
   * @return 1 - ambiguos - two sqls for the same database
   * @return 2 - replace existing generic with new specific
   * @return 3 - sql for rdbms that is currently irrelevant
   * @return 4 - skipped - new is generic, but there is already old specific
   */
  INS_OP gsql_sql_set::insert(const gsql_sql_dscr& a_val)
  {
    if ((a_val.db_type() == db_type_) || (a_val.db_type() == RDBMS::sql))
    { // our database type or generic sql
      uint new_phase = ME::enum_integer<PHASE>(a_val.phase());
      auto found     = sql_map_.find(new_phase);
      if (found != sql_map_.end())
      { // we already have sql descriptor for this phase
        uint old_rdbms = ME::enum_integer<RDBMS>(found->second.db_type());
        uint new_rdbms = ME::enum_integer<RDBMS>(a_val.db_type());
        if (old_rdbms == new_rdbms) return INS_OP::ambigous; //  two definitions for 
                                                             //  the same database
        if (old_rdbms < new_rdbms) //
        {                          // replace generic with specific
          sql_map_[new_phase] = a_val;
          return INS_OP::specific;
        }
        return INS_OP::skip; // new is generic, old is specific, skip the new one
      }
      // no previous input for this phase - insert it
      sql_map_[a_val.key()] = a_val;
      return INS_OP::inserted;
    }
    return INS_OP::other; // sql for not selected (other) rdbms
  }

  const gsql_sql_dscr* gsql_sql_set::fetch(const PHASE& a_phase) const
  {
    uint16_t key = ME::enum_integer<PHASE>(a_phase);
    auto     it  = sql_map_.find(key);
    if (it != sql_map_.end()) { return &(it->second); }
    return nullptr;
  }

  std::string gsql_sql_set::fetch_sql(const PHASE& a_phase) const
  {
    const auto* el = fetch(a_phase);
    return el != nullptr ? trim(el->sql()) : "";
  }

  RDBMS gsql_sql_set::db_type() const { return this->db_type_; }

} // namespace dbgen3