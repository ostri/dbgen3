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
    for (auto b : buf_dscr_) s += out::sl(b.dump("", offs + 2), 0);
    s += out::sl(sql_set_.dump("", offs + 2), 0);
    s += out::sl("}", offs);
    return s;
  }

  bool gsql_q::insert(gsql_sql an_sql) { return sql_set_.insert(an_sql); }

  const gsql_qbuf_dscr& gsql_q::buf_dscr(enum gsql_qbuf_dscr::type ndx) const
  {
    return this->buf_dscr_[static_cast<int>(ndx)];
  }

  void gsql_q::set_buf_dscr(const gsql_qbuf_dscr& buf_dscr)
  {
    assert((buf_dscr.type() == gsql_qbuf_dscr::type::par) ||
           (buf_dscr.type() == gsql_qbuf_dscr::type::res));
    this->buf_dscr_[static_cast<uint>(buf_dscr.type())] = buf_dscr;
  }

} // namespace dbgen3
