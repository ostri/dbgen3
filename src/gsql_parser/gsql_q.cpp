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
    s += sql_set_.dump("", offs + 2);
    s += out::sl("}", offs);
    return s;
  }
  const gsql_qbuf_dscr& gsql_q::buf_dscr(const BUF_TYPE& a_type) const
  {
    return this->buf_dscr_[ME::enum_integer(a_type)];
  }

  void gsql_q::set_buf_dscr(const gsql_qbuf_dscr& buf_dscr)
  {
    assert((buf_dscr.type() == BUF_TYPE::par) ||
           (buf_dscr.type() == BUF_TYPE::res));
    this->buf_dscr_[static_cast<uint>(buf_dscr.type())] = buf_dscr;
  }

} // namespace dbgen3
