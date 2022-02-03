
#include <chrono>

#include "gen_cpp.hpp"
#include "odbc_db.hpp"
#include "enums.hpp"

namespace dbgen3
{
  str_t gen_cpp::define_trivial_getters(const fld_vec& flds, uint max_name_len, uint offs)
  {
    str_t r;
    if (! flds.empty())
    {
      r += out::sl(fmt::format("/// @brief trivial getters"), offs);
      r += out::sl(fmt::format("//@{{"), offs);
      for (const auto& el : flds)
      {
        r += out::sl(
          fmt::format("auto {0:<{1}}() const {{return {0}(0);}}", el.name(), max_name_len), offs);
      }
      r += out::sl(fmt::format("//@}}"), offs);
    }
    return r;
  }
  str_t gen_cpp::define_getters(const fld_vec& flds, uint max_name_len, uint offs)
  {
    str_t r;
    if (! flds.empty())
    {
      r += out::sl(fmt::format("/// @brief getters"), offs);
      r += out::sl(fmt::format("//@{{"), offs);
      for (const auto& el : flds)
      {
        r += out::sl(fmt::format("auto {0:<{1}}(uint ndx) const {{return {0}_.value(ndx);}}",
                                 el.name(),
                                 max_name_len),
                     offs);
      }
      r += out::sl(fmt::format("//@}}"), offs);
    }
    return r;
  }
  str_t gen_cpp::define_trivial_setters(const fld_vec& flds, uint max_name_len, uint offs)
  {
    str_t r;
    str_t type_name;
    if (! flds.empty())
    {
      r += out::sl(fmt::format("/// @brief trivial setters"), offs);
      r += out::sl(fmt::format("//@{{"), offs);
      for (const auto& el : flds)
      {
        int  db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
        auto group       = db::attr_type(db_type_int);
        switch (group)
        {
        case db::ATTR_TYPE::atomic:
        {
          type_name = db::ct_name(db_type_int);
          break;
        }
        case db::ATTR_TYPE::string:
        {
          type_name = "cstr_t";
          break;
        }
        case db::ATTR_TYPE::bstring:
        {
          type_name = db::ct_name(db_type_int);
          break;
        }
        case db::ATTR_TYPE::unknown: throw std::runtime_error("unhandled attr group");
        }
        r += out::sl(
          fmt::format(
            "void set_{0:<{1}}({2:7} v) {{{0}(v, 0);}}", el.name(), max_name_len, type_name),
          offs);
      }
      r += out::sl(fmt::format("//@}}"), offs);
    }
    return r;
  }
  str_t gen_cpp::define_setters(const fld_vec& flds, uint max_name_len, uint offs)
  {
    str_t r;
    str_t type_name;
    if (! flds.empty())
    {
      r += out::sl(fmt::format("/// @brief setters"), offs);
      r += out::sl(fmt::format("//@{{"), offs);
      for (const auto& el : flds)
      {
        int  db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
        auto group       = db::attr_type(db_type_int);
        switch (group)
        {
        case db::ATTR_TYPE::atomic:
        {
          type_name = db::ct_name(db_type_int);
          break;
        }
        case db::ATTR_TYPE::string:
        {
          type_name = "cstr_t";
          break;
        }
        case db::ATTR_TYPE::bstring:
        {
          type_name = db::ct_name(db_type_int);
          break;
        }
        case db::ATTR_TYPE::unknown: throw std::runtime_error("unhandled attr group");
        }
        r += out::sl(fmt::format("void set_{0:<{1}}({2:7} v, uint ndx) {{{0}_.set_value(v, ndx);}}",
                                 el.name(),
                                 max_name_len,
                                 type_name),
                     offs);
      }
      r += out::sl(fmt::format("//@}}"), offs);
    }
    return r;
  }
  str_t gen_cpp::define_dump(const fld_vec& flds, uint max_name_len, uint offs)
  {
    str_t r;
    if (! flds.empty())
    {
      r += out::sl(fmt::format("std::string dump()"), offs);
      r += out::sl(fmt::format("{{"), offs);
      r += out::sl(fmt::format("  std::string r;"), offs);
      for (const auto& el : flds)
      {
        r += out::sl(
          fmt::format("  r += \"{0:<{1}}:\" + {0}_.dump_all() + \"\\n\";", el.name(), max_name_len),
          offs);
      }
      r += out::sl(fmt::format("  return r;"), offs);
      r += out::sl(fmt::format("}}"), offs);
    }
    return r;
  }
  str_t gen_cpp::define_attributes(const fld_vec& flds, uint offs)
  {
    str_t r;
    for (const auto& el : flds)
    {
      auto  db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
      auto  db_type     = db::attr_type(db_type_int);
      auto  ct_name     = db::ct_name(db_type_int);
      str_t ct_name_comma(ct_name);
      ct_name_comma += ',';
      switch (db_type)
      {
      case db::ATTR_TYPE::atomic:
      {
        r += out::sl(
          fmt::format("db::atomic <{:21} {}, N> {}_{{}}; ", ct_name_comma, el.dec(), el.name()),
          offs);
        break;
      }
      case db::ATTR_TYPE::string:
      {
        r += out::sl(fmt::format("db::string <{}, N> {}_{{}}; ", el.width(), el.name()), offs);
        break;
      }
      case db::ATTR_TYPE::bstring:
      {
        r += out::sl(fmt::format("db::bstring <{}, N> {}_{{}}; ", el.width(), el.name()), offs);
        break;
      }
      case db::ATTR_TYPE::unknown:
      {
        throw std::runtime_error("unknown attribute type " + el.dump());
      }
      }
    }
    return r;
  }
  str_t gen_cpp::gen_buf(const gsql_q& q, const BUF_TYPE& a_type, uint offs)
  {
    str_t r;
    auto  bd = q.buf_dscr((a_type));
    if (! bd.should_skip())
    {
      auto        c_name = snake_case(bd.id());
      auto        ml     = bd.max_name_len();
      const auto& flds   = bd.flds();
      r += line(73, offs);
      r += out::sl(fmt::format("template <std::size_t N=1>", c_name), offs);
      r += out::sl(fmt::format("class {}", c_name), offs);
      r += out::sl(fmt::format("{{"), offs);
      r += out::sl(fmt::format("public: /*...public methods...*/"), offs);
      r += define_trivial_getters(flds, ml, offs + 2);
      r += define_getters(flds, ml, offs + 2);
      r += define_trivial_setters(flds, ml, offs + 2);
      r += define_setters(flds, ml, offs + 2);
      r += define_dump(flds, ml, offs + 2);
      r += out::sl(fmt::format("private:/*...private methods & attributes...*/"), offs);
      r += define_attributes(bd.flds(), offs + 2);
      r += out::sl(fmt::format("}}; // class {}", c_name), offs);
    }
    return r;
  }
  str_t gen_cpp::gen_utl(const gsql_q& q, uint offs)
  {
    str_t r;
    auto  sql = snake_case(q.id());
    r += out::sl(r, offs);
    return r;
  }
  str_t gen_cpp::gen_file(uint offs)
  {
    str_t       r;
    auto        now      = std::chrono::system_clock::now();
    std::time_t time     = std::chrono::system_clock::to_time_t(now);
    std::string time_str = std::ctime(&time);

    r += out::sl(fmt::format("/* auto generated '{}' - do not alter */",
                             time_str.substr(0, time_str.size() - 1)),
                 offs);
    r += out::sl("#include <string>", offs);
    r += out::sl("#include <string_view>", offs);
    r += out::sl(fmt::format("#include \"{}\"", "dbgen3_templ.hpp"), offs);
    auto h_ml = set().header_multi_line().lines();
    for (const auto& line:h_ml ) r += line + "\n";
    auto ns = snake_case(set().id());
    r += out::sl(fmt::format("namespace {}", ns), offs);
    r += out::sl("{", offs);
    r += out::sl("  using uint   = std::size_t;", offs);
    r += out::sl("  using str_t  = std::string;", offs);
    r += out::sl("  using cstr_t = std::string_view;", offs);
    for (auto q : set().q_dic())
    { //
      auto ns = snake_case(q.second.id());
      r += line(75, offs + 2, '.');
      r += out::sl("/*", offs + 2);
      r += out::sl(q.second.sql(PHASE::main), offs + 2);
      r += out::sl(" */", offs + 2);
      r += out::sl(fmt::format("namespace {}", ns), offs + 2);
      r += out::sl("{", offs + 2);
      r += gen_buf(q.second, BUF_TYPE::par, offs + 4);
      r += gen_buf(q.second, BUF_TYPE::res, offs + 4);
      r += gen_utl(q.second, offs + 4);
      r += out::sl(fmt::format("}}; //namespace {}", ns), offs + 2);
    }
    r += out::sl(fmt::format("}}; //namespace {}", ns), offs);

    return r;
  }

}; // namespace dbgen3