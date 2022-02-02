
#include <chrono>

#include "gen_cpp.hpp"
#include "odbc_db.hpp"
#include "enums.hpp"

namespace dbgen3
{
  str_t gen_cpp::define_attributes(const fld_vec& flds, uint offs)
  {
    str_t r;
    for (const auto& el : flds)
    {
      auto db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
      auto db_type = db::attr_type(db_type_int);
      auto ct_name = db::ct_name(db_type_int);
      str_t ct_name_comma(ct_name);
      ct_name_comma += ',';
      switch (db_type)
      {
      case db::ATTR_TYPE::atomic:
      {
        r += out::sl(fmt::format("db::atomic <{:21} {}, N> {}_{{}}; ", ct_name_comma, el.dec(), el.name()), offs);
        break;
      }
      case db::ATTR_TYPE::string:
      {
        r += out::sl(fmt::format("db::string <{:21} N> {}_{{{}}}; ", ct_name_comma, el.name(), el.dec()), offs);
        break;
      }
      case db::ATTR_TYPE::bstring:
      {
        r += out::sl(fmt::format("db::bstring<{:21} N> {}_{{{}}}; ", ct_name_comma, el.name(), el.dec()), offs);
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
      auto c_name = snake_case(bd.id());
      r += line(73, offs);
      r += out::sl(fmt::format("template <std::size_t N=1>", c_name), offs);
      r += out::sl(fmt::format("class {}", c_name), offs);
      r += out::sl(fmt::format("{{"), offs);
      r += out::sl(fmt::format("private:"), offs);
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
    r += out::sl(fmt::format("#include \"{}\"", "/home/ostri/dbgen3/src/runtime/dbgen3_templ.hpp"),
                 offs); // FIXME
    auto ns = snake_case(set().id());
    r += out::sl(fmt::format("namespace {}", ns), offs);
    r += out::sl("{", offs);
    for (auto q : set().q_dic())
    { //
      auto ns = snake_case(q.second.id());
      r += line(75, offs + 2, '.');
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