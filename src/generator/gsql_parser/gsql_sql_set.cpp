#include "gsql_parser/gsql_sql_set.hpp"
#include "utility_classes/multi_line.hpp"
#include <stdexcept>

namespace dbgen3
{

  std::string gsql_sql_set::dump() const { return dump("", 0); };

  std::string gsql_sql_set::dump(uint offs) const { return dump("", offs); };

  std::string gsql_sql_set::dump(cstr_t a_msg, uint offs) const
  {
    std::string s;
    if (! a_msg.empty()) s += out::sl(a_msg, offs);
    s += out::sl("sql_set:", offs);
    s += out::sl("{", offs);
    s += sql_dscr_.dump(offs + 2);
    s += out::sl("}", offs);
    return s;
  };
  /**
   * @brief insert the sql descriptor to the structure
   *
   * The metod tries to insert the SQL descriptior into the structure.
   * We are getting only sql descriptors that belongs to the target rdbms or generic SQL.
   *
   * Alg:
   * try fetch record with the same key (i.e. RDBMS (generic or target))
   * there is existing old value
   * - yes
   *   - both rdbms descriptors are the same => (ambiguous)
   *   - new descriptor is more specific => replace old with new (specific)
   *   - old descriptor is more specific => keep it (skip)
   * - no
   *   -insert new descriptor (insert)
   *
   * @param a_sql_dscr sql descriptor
   * @return inserted
   * @return ambiguos - two sqls for the same database
   * @return replace existing generic with new specific
   * @return skipped - new is generic, but there is already old specific
   */
  INS_OP gsql_sql_set::put(const gsql_sql_dscr& a_val)
  {
    if (! empty_)
    {
      auto old_sql = sql_dscr_.db_type();
      auto new_sql = a_val.db_type();
      if (old_sql == new_sql) return INS_OP::ambiguous;
      if (new_sql > old_sql)
      {
        sql_dscr_ = a_val;
        return INS_OP::specific;
      }
      return INS_OP::skip;
    }
    empty_    = false;
    sql_dscr_ = a_val;
    return INS_OP::inserted;
  }

  std::string       gsql_sql_set::sql() const { return sql_dscr_.sql(); }
  std::string       gsql_sql_set::prep_sql() const { return sql_dscr_.sql_prep(); }
  const multi_line& gsql_sql_set::sql_ml() const { return sql_dscr_.sql_ml(); }
  const multi_line& gsql_sql_set::prep_sql_ml() const { return sql_dscr_.sql_prep_ml(); }
  RDBMS             gsql_sql_set::db_type() const { return this->db_type_; }

} // namespace dbgen3