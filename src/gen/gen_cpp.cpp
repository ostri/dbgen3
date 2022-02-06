
#include <chrono>

#include "fld_dscr.hpp"
#include "enums.hpp"
#include "gen_cpp.hpp"
#include "odbc_db.hpp"

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
        int  db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
        auto group       = db::attr_type(db_type_int);
        switch (group)
        {
        case db::ATTR_TYPE::atomic:
        {
          const auto* type_name = db::ct_name(db_type_int);
          r += out::sl(fmt::format("{2:24} {0:<{1}}() const {{return {0}(0);}}", //
                                   el.name(),
                                   max_name_len,
                                   type_name),
                       offs);
          break;
        }
        case db::ATTR_TYPE::string:
        {
          r += out::sl(fmt::format("{2:24} {0:<{1}}() const {{return {0}(0);}}", //
                                   el.name(),
                                   max_name_len,
                                   "cstr_t"),
                       offs);
          break;
        }
        case db::ATTR_TYPE::bstring:
        {
          /// FIXME from EL -> el-name
          const auto* type_name = "const uint8_t";
          r += out::sl(fmt::format("std::span<{2}> {0:<{1}}() const {{return {0}(0);}}", //
                                   el.name(),
                                   max_name_len,
                                   type_name),
                       offs);
          break;
        }
        case db::ATTR_TYPE::unknown:
          throw std::runtime_error(str_t("Unexpected type unknown") + __FILE__ +
                                   std::to_string(__LINE__));
        }
      }
      r += out::sl(fmt::format("//@}}"), offs);
    }
    return r;
  }
  /**
   * @brief define getters
   *
   * @param flds          buffer attributes
   * @param max_name_len  max length of the attribute name
   * @param offs          offset from left
   * @return str_t
   */
  str_t gen_cpp::define_getters(const fld_vec& flds, uint max_name_len, uint offs)
  {
    str_t r;
    if (! flds.empty())
    {
      r += out::sl(fmt::format("/// @brief getters"), offs);
      r += out::sl(fmt::format("//@{{"), offs);
      for (const auto& el : flds)
      {
        int  db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
        auto group       = db::attr_type(db_type_int);
        switch (group)
        {
        case db::ATTR_TYPE::atomic:
        {
          const auto* type_name = db::ct_name(db_type_int);
          r += out::sl(fmt::format("{2:24} {0:<{1}}(uint ndx) const {{return {0}_.value(ndx);}}",
                                   el.name(),
                                   max_name_len,
                                   type_name),
                       offs);
          break;
        }
        case db::ATTR_TYPE::string:
        {
          r += out::sl(fmt::format("{2:24} {0:<{1}}(uint ndx) const {{return {0}_.value(ndx);}}",
                                   el.name(),
                                   max_name_len,
                                   "cstr_t"),
                       offs);
          break;
        }
        case db::ATTR_TYPE::bstring:
        {
          const auto* type_name = "const uint8_t";
          r += out::sl(fmt::format("std::span<{2}> {0:<{1}}(uint ndx) const {{return {0}_.value(ndx);}}",
                                   el.name(),
                                   max_name_len,
                                   type_name),
                       offs);
          break;
        }
        case db::ATTR_TYPE::unknown: throw std::runtime_error("Unexpected type unknown");
        }
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
          type_name = "std::span<uint8_t>";
          break;
        }
        case db::ATTR_TYPE::unknown: throw std::runtime_error("unhandled attr group");
        }
        r += out::sl(
          fmt::format(
            "void set_{0:<{1}}({2:7} v) {{set_{0}(v, 0);}}", el.name(), max_name_len, type_name),
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
          r +=
            out::sl(fmt::format("void set_{0:<{1}}({2:7} v, uint ndx) {{{0}_.set_value(v, ndx);}}",
                                el.name(),
                                max_name_len,
                                type_name),
                    offs);
          break;
        }
        case db::ATTR_TYPE::string:
        {
          type_name = "cstr_t";
          r +=
            out::sl(fmt::format("void set_{0:<{1}}({2:7} v, uint ndx) {{{0}_.set_value(v, ndx);}}",
                                el.name(),
                                max_name_len,
                                type_name),
                    offs);
          break;
        }
        case db::ATTR_TYPE::bstring:
        {
          type_name = "std::span<uint8_t>";
          r +=
            out::sl(fmt::format("void set_{0:<{1}}({2:7} v, uint ndx) {{{0}_.set_value(v, ndx);}}",
                                el.name(),
                                max_name_len,
                                type_name),
                    offs);
          break;
        }
        case db::ATTR_TYPE::unknown: throw std::runtime_error("unhandled attr group");
        }
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
      r += out::sl(fmt::format("str_t dump() override"), offs);
      r += out::sl(fmt::format("{{"), offs);
      r += out::sl(fmt::format("  std::string r;"), offs);
      for (const auto& el : flds)
      {
        r += out::sl(fmt::format(R"(  r += "{0:<{1}}:" + {0}_.dump_all("",0,0) + "\n";)",
                                 el.name(),
                                 max_name_len),
                     offs);
      }
      r += out::sl(fmt::format("  return r;"), offs);
      r += out::sl(fmt::format("}}"), offs);
    }
    return r;
  }
  str_t gen_cpp::define_attributes_const(const fld_vec& flds, uint max_name_len, uint offs)
  {
    str_t r;
    for (const auto& el : flds)
    {
      auto db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
      auto db_type     = db::attr_type(db_type_int);
      switch (db_type)
      {
      case db::ATTR_TYPE::atomic:
      {
        r += out::sl(
          fmt::format("constexpr static const uint {0:<1}_dec = {2}; // number of decimal places",
                      el.name(),
                      max_name_len,
                      el.dec()),
          offs);
        break;
      }
      case db::ATTR_TYPE::string:
      case db::ATTR_TYPE::bstring:
      {
        auto const_name = el.name() + "_len";
        r +=
          out::sl(fmt::format("constexpr static const uint {1:{2}} = {0}; // max (b) string length",
                              el.width(),
                              const_name,
                              max_name_len + 4),
                  offs);
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

  str_t gen_cpp::define_attributes(const fld_vec& flds, uint max_name_len, uint offs)
  {
    str_t r;
    for (const auto& el : flds)
    {
      auto        db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
      auto        db_type     = db::attr_type(db_type_int);
      const auto* ct_name     = db::ct_name(db_type_int);
      const auto* db_name     = db::dbt_name(db_type_int);
      switch (db_type)
      {
      case db::ATTR_TYPE::atomic:
      {
        r += out::sl(fmt::format("db::atomic <{0:<1}, {2}, {4}_dec, N> {4}_{{}}; ",
                                 ct_name,
                                 max_name_len,
                                 db_name,
                                 el.dec(),
                                 el.name()),
                     offs);
        break;
      }
      case db::ATTR_TYPE::string:
      {
        r += out::sl(
          fmt::format("db::string <std::array<{2}, {0}_len>, {1}, N, {2}> {0}_{{}}; ", el.name(), db_name, "char"), offs);
        break;
      }
      case db::ATTR_TYPE::bstring:
      {
        r += out::sl(
          fmt::format("db::bstring<std::array<{2}, {0}_len>, {1}, N, {2}> {0}_{{}}; ", el.name(), db_name, "uint8_t"),
          offs);
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
  /**
   * @brief
   *
   * @param flds
   * @param offs
   * @return str_t
   */
  str_t gen_cpp::define_attr_array(const fld_vec& flds, uint /*unused*/, uint offs)
  {
    str_t r;
    r += out::sl(fmt::format("constexpr static const uint max_attr={};", flds.size()), offs);
    r += out::sl(fmt::format("std::array<db::attr_root_root<N>*, max_attr> attr_"), offs);
    r += out::sl(fmt::format("{{"), offs);
    for (const auto& attr : flds)
      //      r += out::sl(fmt::format("reinterpret_cast<db::attr_root*>(&{}_),", attr.name()), offs
      //      + 2);
      r += out::sl(fmt::format("&{}_,", attr.name()), offs + 2);
    r.resize(r.size() - 2); // eliminate comma and space
    r += out::sl(fmt::format(""), offs);
    r += out::sl(fmt::format("}};"), offs);
    return r;
    // for (const auto& el : flds)
    // {
    //   auto        db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
    //   auto        db_type     = db::attr_type(db_type_int);
    //   const auto* ct_name     = db::ct_name(db_type_int);
    //   const auto* db_name     = db::dbt_name(db_type_int);
    //   switch (db_type)
    //   {
    //   case db::ATTR_TYPE::atomic:
    //   {
    //     r += out::sl(fmt::format("db::atomic <{0:<1}, {2}, {4}_dec, N> {4}_{{}}; ",
    //                              ct_name,
    //                              max_name_len,
    //                              db_name,
    //                              el.dec(),
    //                              el.name()),
    //                  offs);
    //     break;
    //   }
    //   case db::ATTR_TYPE::string:
    //   {
    //     r += out::sl(fmt::format("db::string <{0}_len, N, uint8_t> {0}_{{}}; ", el.name()),
    //     offs); break;
    //   }
    //   case db::ATTR_TYPE::bstring:
    //   {
    //     r += out::sl(fmt::format("db::bstring <{0}_len, N> {0}_{{}}; ", el.name()), offs);
    //     break;
    //   }
    //   case db::ATTR_TYPE::unknown:
    //   {
    //     throw std::runtime_error("unknown attribute type " + el.dump());
    //   }
    //   }
    // }
    // return r;
  }
  str_t gen_cpp::gen_buf(const gsql_q& q, const BUF_TYPE& a_type, uint offs)
  {
    str_t       r;
    const auto& bd = q.buf_dscr((a_type));
    if (! bd.should_skip())
    {
      auto              c_name      = snake_case(bd.id());
      auto              ml          = bd.max_name_len();
      const auto&       flds        = bd.flds();
      const static uint line_length = 73;
      r += line(line_length, offs);
      r += out::sl(fmt::format("template <std::size_t N=1>", c_name), offs);
      r += out::sl(fmt::format("class {} : public db::buffer_root", c_name), offs);
      r += out::sl(fmt::format("{{"), offs);
      r += out::sl(fmt::format("public: /*...public methods...*/"), offs);
      r += define_trivial_getters(flds, ml, offs + 2);
      r += define_getters(flds, ml, offs + 2);
      r += define_trivial_setters(flds, ml, offs + 2);
      r += define_setters(flds, ml, offs + 2);
      r += define_dump(flds, ml, offs + 2);
      r += out::sl(fmt::format("private:/*...private methods & attributes...*/"), offs);
      r += define_attributes_const(bd.flds(), ml, offs + 2);
      r += define_attributes(bd.flds(), ml, offs + 2);
      r += define_attr_array(bd.flds(), ml, offs + 2);
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
    str_t                           r;
    auto                            now           = std::chrono::system_clock::now();
    std::time_t                     time          = std::chrono::system_clock::to_time_t(now);
    constexpr static const uint     ctime_buf_len = 26;
    std::array<char, ctime_buf_len> buf{};
    std::string                     time_str = ctime_r(&time, buf.data());

    r += out::sl(fmt::format("/* auto generated '{}' - do not alter */",
                             time_str.substr(0, time_str.size() - 1)),
                 offs);
    r += out::sl("#include <array>", offs);
    r += out::sl("#include <span>", offs);
    r += out::sl("#include <string>", offs);
    r += out::sl("#include <string_view>", offs);
    r += out::sl("", offs);
    r += out::sl(fmt::format("#include \"{}\"", "dbgen3_templ.hpp"), offs);
    auto h_ml = set().header_multi_line().lines();
    for (const auto& line : h_ml) r += line + "\n";
    auto ns = snake_case(set().id());
    r += out::sl(fmt::format("namespace {}", ns), offs);
    r += out::sl("{", offs);
    r += out::sl("  using uint    = std::size_t;", offs);
    r += out::sl("  using str_t   = std::string;", offs);
    r += out::sl("  using cstr_t  = std::string_view;", offs);
    r += out::sl("  using bstr_t  = std::span<uint8_t>;", offs);
    r += out::sl("  using cbstr_t = std::span<const uint8_t>;", offs);
    for (const auto& q : set().q_dic())
    { //
      auto              ns          = snake_case(q.second.id());
      const static uint line_length = 75;
      r += line(line_length, offs + 2, '.');
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