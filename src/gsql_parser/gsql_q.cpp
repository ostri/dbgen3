#include "gsql_q.hpp"
#include "gen.hpp"
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
  /**
   * @brief does SQL demands a buffer (PAR/RES)
   *
   * @param a_type type of buffer (PAR/RES)
   * @return true SQL requires the buffer
   * @return false SQL does not mandate the buffer
   */
  bool gsql_q::must_have_buf(const db::BUF_TYPE& a_type) const
  {
    return ! buf_dscr(a_type).must_generate();
  }
  void gsql_q::set_id(cstr_t o) { id_ = o; }
  gsql_q::gsql_q(const RDBMS& a_db_type)
  : sql_set_(a_db_type){};
  cstr_t                gsql_q::id() const { return id_; }
  q_buf_arr&            gsql_q::buf_dscr() { return buf_dscr_; }
  str_t                 gsql_q::namespace_str() const { return snake_case(id_); }
  const gsql_qbuf_dscr& gsql_q::buf_dscr(const db::BUF_TYPE& a_type) const
  {
    return this->buf_dscr_[ME::enum_integer(a_type)];
  }

  q_buf_arr&            gsql_q::buf() { return buf_dscr_; }

  const q_buf_arr&      gsql_q::buf() const { return buf_dscr_; }

  // std::string           sql() const;
  std::string gsql_q::sql() const { return sql_set_.sql(); };

  std::string gsql_q::sql_prep() const { return sql_set_.prep_sql(); };

  RDBMS gsql_q::db_type() const { return sql_set_.db_type(); }

  void gsql_q::set_buf_dscr(const gsql_qbuf_dscr& buf_dscr)
  { // NOLINTNEXTLINE clang-tidy(hicpp-no-array-decay)
    assert((buf_dscr.type() == db::BUF_TYPE::par) || (buf_dscr.type() == db::BUF_TYPE::res));
    this->buf_dscr_[static_cast<uint>(buf_dscr.type())] = buf_dscr;
  }

  void gsql_q::set_sql_set(const gsql_sql_set& o) { sql_set_ = o; }


  const gsql_sql_set& gsql_q::sql_set() const { return sql_set_; }

  const int_vec& gsql_q::allowed() const { return this->allowed_; }

  void gsql_q::set_allowed(const int_vec& allowed) { this->allowed_ = allowed; }

  void gsql_q::set_allowed(cstr_t allowed)
  {
    std::stringstream iss(str_t(allowed.data(), allowed.size()));
    int               number;
    while (iss >> number) allowed_.push_back(number);
    std::sort(allowed_.begin(), allowed_.end());
  }

} // namespace dbgen3
