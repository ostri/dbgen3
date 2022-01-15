#include "gsql_qbuf_dscr.hpp"
namespace dbgen3
{
  std::string   gsql_qbuf_dscr::dump() const { return dump(0); }

  std::string gsql_qbuf_dscr::dump(uint offs) const { return dump("", offs); }

  std::string gsql_qbuf_dscr::dump(const std::string& a_msg, uint offs) const
  {
    std::string s;
    s += out::s(a_msg + type_str(type_) + ": {", offs);
    s += out::s("id: '" + id_ + "' ", 0);
    s += out::s("skip: '" + std::string(skip_ ? "true" : "false") + "' ", 0);
    s += out::s("}", 0);
    return s;
  }

  const std::string& gsql_qbuf_dscr::type_str(enum type a_type) const {return type_str_[static_cast<uint>(a_type)];}

} // namespace dbgen3
