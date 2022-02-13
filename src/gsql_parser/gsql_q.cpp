#include "gsql_q.hpp"
#include "string_format.hpp"
namespace dbgen3
{

  std::string gsql_q::dump() const { return dump("", 0); }
  std::string gsql_q::dump(uint offs) const { return dump("", offs); }

  std::string gsql_q::dump(const std::string& a_msg, uint offs) const
  {
    std::string s;
    if (! a_msg.empty()) s += out::s(a_msg, offs);
    s += out::sl("query", offs);
    s += out::sl("{", offs);
    s += out::sl("  id: '" + id_ + "'", offs);
    for (const auto& b : buf_dscr_) s += b.dump("", offs + 2);
    s += sql_set_.dump("", offs + 2);
    s += out::sl("}", offs);
    return s;
  }
  void gsql_q::set_id(cstr_t o) { id_ = o; }
  gsql_q::gsql_q(const RDBMS& a_db_type): sql_set_(a_db_type){};
  cstr_t            gsql_q::id() const { return id_; }
  const gsql_qbuf_dscr& gsql_q::buf_dscr(const db::BUF_TYPE& a_type) const
  {
    return this->buf_dscr_[ME::enum_integer(a_type)];
  }

  std::string gsql_q::sql(const PHASE& a_phase) const
  {
    return sql_set_.fetch_sql(a_phase);
  }

  RDBMS                 gsql_q::db_type() const {return sql_set_.db_type();}

  void gsql_q::set_buf_dscr(const gsql_qbuf_dscr& buf_dscr)
  { // NOLINTNEXTLINE clang-tidy(hicpp-no-array-decay)
    assert((buf_dscr.type() == db::BUF_TYPE::par) ||
           (buf_dscr.type() == db::BUF_TYPE::res));
    this->buf_dscr_[static_cast<uint>(buf_dscr.type())] = buf_dscr;
  }

  void gsql_q::set_sql_set(const gsql_sql_set& o) { sql_set_ = o; }

  // void gsql_q::set_buf_dscr_flds(const db::BUF_TYPE& a_bt, const fld_vec& a_fld_vec)
  // {
  //   buf_dscr_[ME::enum_integer<db::BUF_TYPE>(a_bt)].set_flds(a_fld_vec);
  // }

  const gsql_sql_set& gsql_q::sql_set() const { return sql_set_; }

  uint                gsql_q::size() const { return sql_set_.size(); }

} // namespace dbgen3
