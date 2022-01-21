#include "gsql_qbuf_dscr.hpp"
namespace dbgen3
{
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
} // namespace dbgen3
