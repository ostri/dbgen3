#include "gsql_qbuf_dscr.hpp"
namespace dbgen3
{
  std::string gsql_qbuf_dscr::dump() const { return dump(0); }

  std::string gsql_qbuf_dscr::dump(uint offs) const { return dump("", offs); }

  std::string gsql_qbuf_dscr::dump(const std::string& a_msg, uint offs) const
  {
    std::string s;
    if (!a_msg.empty()) s += out::s(a_msg, offs);
    s += out::s(fmt::format("{}: {{ id: '{}' skip: {} }}", 
    ME::enum_name(type_), id_, skip_), offs);
    return s;
  }
} // namespace dbgen3
