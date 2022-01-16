#include "gsql_qbuf_dscr.hpp"
namespace dbgen3
{
  std::string gsql_qbuf_dscr::dump() const { return dump(0); }

  std::string gsql_qbuf_dscr::dump(uint offs) const { return dump("", offs); }

  std::string gsql_qbuf_dscr::dump(const std::string& a_msg, uint offs) const
  {
    std::string s;
    s += out::s(fmt::format("{} {}: {}", a_msg, ME::enum_name(type_), '{'), offs);
    s += out::s(fmt::format("id: '{}'", id_), 0);
    s += out::s("skip: '" + std::string(skip_ ? "true" : "false") + "' ", 0);
    s += out::s("}", 0);
    return s;
  }
} // namespace dbgen3
