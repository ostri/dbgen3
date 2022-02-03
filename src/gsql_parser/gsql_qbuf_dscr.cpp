#include "gsql_qbuf_dscr.hpp"
namespace dbgen3
{
  uint           gsql_qbuf_dscr::max_name_len() const { return this->max_name_len_; }

  std::string gsql_qbuf_dscr::dump() const { return dump(0); }

  std::string gsql_qbuf_dscr::dump(uint offs) const { return dump("", offs); }

  std::string gsql_qbuf_dscr::dump(const std::string& a_msg, uint offs) const
  {
    std::string s;
    if (!a_msg.empty()) s += out::s(a_msg, offs);
    s += out::sl(fmt::format("{}:", ME::enum_name(type_)), offs);
    s += out::sl("{", offs);
    s += out::sl(fmt::format("  id: '{}' skip: {}", id_, skip_), offs);
    s += out::sl("  flds: ",offs);
    s += out::sl("  {", offs);
    for (const auto& fld: flds_) s += fld.dump(offs+4);
    s += out::sl("  }", offs);
    s += out::sl("}", offs);
    return s;
  }

  uint gsql_qbuf_dscr::calc_max_name_length(const fld_vec& vec)
  {
    uint len=0;
    for (const auto& fld:vec)
    if (fld.name().size()>len) len = fld.name().size();
    return len;
  }
} // namespace dbgen3
