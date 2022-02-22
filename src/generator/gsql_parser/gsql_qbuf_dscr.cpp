#include "gsql_qbuf_dscr.hpp"
#include "magic_enum.hpp"
#include "odbc_db.hpp"
namespace dbgen3
{
  gsql_qbuf_dscr::gsql_qbuf_dscr(const db::BUF_TYPE& a_type, bool should_skip)
  : gsql_qbuf_dscr(a_type, ME::enum_name<db::BUF_TYPE>(a_type), should_skip)
  { }

  std::string     gsql_qbuf_dscr::class_name() const { return snake_case(id_); }

  cstr_t          gsql_qbuf_dscr::names(std::size_t ndx) const { return names_.at(ndx); }

  bool            gsql_qbuf_dscr::must_generate() const { return ! skip_ && ! flds_.empty(); }

  bool            gsql_qbuf_dscr::is_required() const { return ! flds_.empty(); }

  uint gsql_qbuf_dscr::max_name_len() const { return this->max_name_len_; }
  uint gsql_qbuf_dscr::max_ctype_len() const { return this->max_ctype_len_; }

  std::string gsql_qbuf_dscr::dump() const { return dump(0); }

  std::string gsql_qbuf_dscr::dump(uint offs) const { return dump("", offs); }

  std::string gsql_qbuf_dscr::dump(const std::string& a_msg, uint offs) const
  {
    std::string s;
    if (! a_msg.empty()) s += out::s(a_msg, offs);
    s += out::sl(fmt::format("{}:", ME::enum_name(type_)), offs);
    s += out::sl("{", offs);
    s += out::sl(fmt::format("  id: '{}' skip: {}", id_, skip_), offs);
    s += out::sl("  flds: ", offs);
    s += out::sl("  {", offs);
    for (const auto& fld : flds_) s += fld.dump(offs + 4);
    s += out::sl("  }", offs);
    s += out::sl("}", offs);
    return s;
  }

  void gsql_qbuf_dscr::set_names(cstr_t names) { names_ = split(names, ' '); }


  uint gsql_qbuf_dscr::calc_max_name_length(const fld_vec& vec)
  {
    uint len = 0;
    for (const auto& fld : vec)
      if (fld.name().size() > len) len = fld.name().size();
    return len;
  }
  uint gsql_qbuf_dscr::calc_max_ctype_length(const fld_vec& vec)
  {
    uint len = 0;
    for (const auto& fld : vec)
    {
      auto c_type_len = 0UL;
      auto odbc_type_int = ME::enum_integer<ODBC_TYPE>(fld.type());
      c_type_len = str_t(db::ct_name(odbc_type_int)).size();
      if (c_type_len > len) len = c_type_len;
    }
    return len;
  }
} // namespace dbgen3
