
#include <chrono>
#include <magic_enum.hpp>

#include "fld_dscr.hpp"
#include "enums.hpp"
#include "fmt/core.h"
#include "gen_cpp.hpp"
#include "odbc_db.hpp"

namespace dbgen3
{
  str_t gen_cpp::define_trivial_null_getters(const fld_vec& flds, uint max_name_len, uint offs)
  {
    str_t r;
    if (! flds.empty())
    {
      r += out::sl(fmt::format("/// @brief trivial null getters"), offs);
      r += out::sl(fmt::format("//@{{"), offs);
      for (const auto& el : flds)
      {
        //        int         db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
        //        const auto* type_name   = db::ct_name(db_type_int);
        r += out::sl(fmt::format("bool is_null_{0:<{1}}() const {{return is_null_{0}(0);}}", //
                                 el.name(),
                                 max_name_len //,
                                              //  type_name,
                                              //  max_ctype_len
                                 ),
                     offs);
      }
      r += out::sl(fmt::format("//@}}"), offs);
    }
    return r;
  }
  str_t gen_cpp::define_trivial_getters(const fld_vec& flds,
                                        uint           max_name_len,
                                        uint           max_ctype_len,
                                        uint           offs)
  {
    str_t r;
    if (! flds.empty())
    {
      r += out::sl(fmt::format("/// @brief trivial getters"), offs);
      r += out::sl(fmt::format("//@{{"), offs);
      for (const auto& el : flds)
      {
        int         db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
        const auto* type_name   = db::ct_name(db_type_int);
        r += out::sl(fmt::format("{2:<{3}} {0:<{1}}() const {{return {0}(0);}}", //
                                 el.name(),
                                 max_name_len,
                                 type_name,
                                 max_ctype_len),
                     offs);
      }
      r += out::sl(fmt::format("//@}}"), offs);
    }
    return r;
  }
  /**
   * @brief define null getters
   *
   * @param flds          buffer attributes
   * @param max_name_len  max length of the attribute name
   * @param offs          offset from left
   * @return str_t
   */
  str_t gen_cpp::define_null_getters(const fld_vec& flds, uint max_name_len, uint offs)
  {
    str_t r;
    if (! flds.empty())
    {
      r += out::sl(fmt::format("/// @brief null getters"), offs);
      r += out::sl(fmt::format("//@{{"), offs);
      for (const auto& el : flds)
      {
        //        int         db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
        //        const auto* type_name   = db::ct_name(db_type_int);
        r +=
          out::sl(fmt::format("bool is_null_{0:<{1}}(uint ndx) const {{return {0}_.is_null(ndx);}}",
                              el.name(),
                              max_name_len //,
                                           //                                 type_name,
                                           //                                 max_ctype_len
                              ),
                  offs);
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
  str_t gen_cpp::define_getters(const fld_vec& flds,
                                uint           max_name_len,
                                uint           max_ctype_len,
                                uint           offs)
  {
    str_t r;
    if (! flds.empty())
    {
      r += out::sl(fmt::format("/// @brief getters"), offs);
      r += out::sl(fmt::format("//@{{"), offs);
      for (const auto& el : flds)
      {
        int         db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
        const auto* type_name   = db::ct_name(db_type_int);
        r += out::sl(fmt::format("{2:<{3}} {0:<{1}}(uint ndx) const {{return {0}_.value(ndx);}}",
                                 el.name(),
                                 max_name_len,
                                 type_name,
                                 max_ctype_len),
                     offs);
      }
      r += out::sl(fmt::format("//@}}"), offs);
    }
    return r;
  }
  str_t gen_cpp::define_trivial_null_setters(const fld_vec& flds,
                                             uint           max_name_len,
                                             uint /*max_ctype_len*/,
                                             uint offs)
  {
    str_t r;
    str_t type_name;
    if (! flds.empty())
    {
      r += out::sl(fmt::format("/// @brief trivial null setters"), offs);
      r += out::sl(fmt::format("//@{{"), offs);
      for (const auto& el : flds)
      {
        int db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
        type_name       = db::ct_name(db_type_int);
        r += out::sl(fmt::format("void set_null_{0:<{1}}() {{set_null_{0}(0);}}",
                                 el.name(),
                                 max_name_len //,
                                 // type_name,
                                 // max_ctype_len
                                 ),
                     offs);
      }
      r += out::sl(fmt::format("//@}}"), offs);
    }
    return r;
  }
  str_t gen_cpp::define_trivial_setters(const fld_vec& flds,
                                        uint           max_name_len,
                                        uint           max_ctype_len,
                                        uint           offs)
  {
    str_t r;
    str_t type_name;
    if (! flds.empty())
    {
      r += out::sl(fmt::format("/// @brief trivial setters"), offs);
      r += out::sl(fmt::format("//@{{"), offs);
      for (const auto& el : flds)
      {
        int db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
        type_name       = db::ct_name(db_type_int);
        r += out::sl(fmt::format("void set_{0:<{1}}({2:<{3}} v) {{set_{0}(v, 0);}}",
                                 el.name(),
                                 max_name_len,
                                 type_name,
                                 max_ctype_len),
                     offs);
      }
      r += out::sl(fmt::format("//@}}"), offs);
    }
    return r;
  }
  str_t gen_cpp::define_null_setters(const fld_vec& flds,
                                     uint           max_name_len,
                                     uint           max_ctype_len,
                                     uint           offs)
  {
    str_t r;
    str_t type_name;
    if (! flds.empty())
    {
      r += out::sl(fmt::format("/// @brief null setters"), offs);
      r += out::sl(fmt::format("//@{{"), offs);
      for (const auto& el : flds)
      {
        int db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
        type_name       = db::ct_name(db_type_int);
        r += out::sl(fmt::format("void set_null_{0:<{1}}(uint ndx) {{{0}_.set_null(ndx);}}",
                                 el.name(),
                                 max_name_len,
                                 type_name,
                                 max_ctype_len),
                     offs);
      }
      r += out::sl(fmt::format("//@}}"), offs);
    }
    return r;
  }
  str_t gen_cpp::define_setters(const fld_vec& flds,
                                uint           max_name_len,
                                uint           max_ctype_len,
                                uint           offs)
  {
    str_t r;
    str_t type_name;
    if (! flds.empty())
    {
      r += out::sl(fmt::format("/// @brief setters"), offs);
      r += out::sl(fmt::format("//@{{"), offs);
      for (const auto& el : flds)
      {
        int db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
        type_name = db::ct_name(db_type_int);
        r +=
          out::sl(fmt::format("void set_{0:<{1}}({2:{3}} v, uint ndx) {{{0}_.set_value(v, ndx);}}",
                              el.name(),
                              max_name_len,
                              type_name,
                              max_ctype_len),
                  offs);
      }
      r += out::sl(fmt::format("//@}}"), offs);
    }
    return r;
  }

  str_t gen_cpp::define_allowed_codes(const int_vec& allowed_codes, uint offs)
  {
    str_t r;
    r += out::sl("/// return codes that do not triger an exception", offs);
    r += out::sl(fmt::format("std::vector<int> allowed_codes()const override"), offs);
    r += out::sl(fmt::format("  {{return {{{0}}};}} //NOLINT "
                             "clang-tidy(cppcoreguidelines-avoid-magic-numbers)",
                             fmt::join(allowed_codes, ", ")),
                 offs);
    return r;
  }
  str_t gen_cpp::define_ID_const(const fld_vec& flds, uint /*max_name_len*/, uint offs)
  {
    str_t r;
    for (auto cnt = 0UL; cnt < flds.size(); cnt++)
    {
      r += out::sl(fmt::format("constexpr static const uint Id{0:02} = {0:2}; // attribute id {1}",
                               cnt,
                               flds[cnt].name()),
                   offs);
    }
    return r;
  }

  str_t gen_cpp::define_attributes_const(const fld_vec& flds, uint /*max_name_len*/, uint offs)
  {
    str_t r;
    auto  ndx = 0;
    for (const auto& el : flds)
    {
      auto  db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
      auto  db_type     = db::attr_type(db_type_int);
      str_t type_name   = fmt::format("T_{0:0{1}}", ndx, 2);
      str_t dec_name    = fmt::format("dec_{0:0{1}}", ndx, 2);
      str_t len_name    = fmt::format("len_{0:0{1}}", ndx, 2);
      switch (db_type)
      {
      case db::ATTR_TYPE::atomic:
      {
        r += out::sl(
          fmt::format("constexpr static const uint {1} = {0:2}; // number of decimal places",
                      el.dec(),
                      dec_name),
          offs);
        break;
      }
      case db::ATTR_TYPE::string:
      {
        r += out::sl(
          fmt::format("constexpr static const uint {1} = {0:2}; // number of decimal places",
                      el.dec(),
                      dec_name),
          offs);
        [[fallthrough]];
      }
      case db::ATTR_TYPE::bstring:
      {
        r +=
          out::sl(fmt::format("constexpr static const uint {1} = {0:2}; // width of the attribute",
                              el.width(),
                              len_name),
                  offs);
        break;
      }
      case db::ATTR_TYPE::unknown:
      {
        throw std::runtime_error("unknown attribute type " + el.dump());
      }
      }
      ++ndx;
    }
    return r;
  }
  str_t gen_cpp::define_attr_types(const fld_vec& flds, uint /*max_type_len*/, uint offs)
  {
    str_t r;
    auto  ndx = 0;
    str_t attr_type;
    for (const auto& el : flds)
    {
      auto  db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
      auto  db_type     = db::attr_type(db_type_int);
      str_t ct_name     = db::ct_name(db_type_int);
      str_t type_name   = fmt::format("T_{0:0{1}}", ndx, 2);
      str_t dec_name    = fmt::format("dec_{0:0{1}}", ndx, 2);
      str_t len_name    = fmt::format("len_{0:0{1}}", ndx, 2);
      switch (db_type)
      {
      case db::ATTR_TYPE::atomic:
      {
        attr_type = ct_name + ";";
        break;
      }
      case db::ATTR_TYPE::string:
      {
        attr_type = fmt::format("std::array<{0}, {1} + 1>;", "char", len_name);
        break;
      }
      case db::ATTR_TYPE::bstring:
      {
        attr_type = fmt::format("std::array<{0}, {1}>;", "uint8_t", len_name);
        break;
      }
      case db::ATTR_TYPE::unknown:
      {
        throw std::runtime_error("unknown attribute type " + el.dump());
      }
      }
      r += out::sl(fmt::format("using {0} = {1}", type_name, attr_type), offs);
      ++ndx;
    }
    return r;
  }

  str_t gen_cpp::define_attributes(const fld_vec& flds, uint /*max_name_len*/, uint offs)
  {
    str_t r;
    auto  ndx = 0;
    for (const auto& el : flds)
    {
      auto        db_type_int = ME::enum_integer<ODBC_TYPE>(el.type());
      auto        db_type     = db::attr_type(db_type_int);
      const auto* db_name     = db::dbt_name(db_type_int);
      str_t       type_name   = fmt::format("T_{0:0{1}}", ndx, 2);
      str_t       dec_name    = fmt::format("dec_{0:0{1}}", ndx, 2);
      str_t       len_name    = fmt::format("dec_{0:0{1}}", ndx, 2);
      switch (db_type)
      {
      case db::ATTR_TYPE::atomic:
      {
        r += out::sl(
          fmt::format(
            "db::atomic <{0}, {1:18}, N, {3}> {2}_{{}}; ", type_name, db_name, el.name(), dec_name),
          offs);
        break;
      }
      case db::ATTR_TYPE::string:
      {
        r += out::sl(fmt::format("db::string <{2}, {1:18}, N, {4}, {3}> {0}_{{}};",
                                 el.name(),
                                 db_name,
                                 type_name,
                                 "char",
                                 dec_name),
                     offs);
        break;
      }
      case db::ATTR_TYPE::bstring:
      { // FIXME we don't need decimal places with bstring
        r += out::sl(fmt::format("db::bstring<{2}, {1:18}, N, {3}> {0}_{{}}; ",
                                 el.name(),
                                 db_name,
                                 type_name,
                                 "uint8_t"),
                     offs);
        break;
      }
      case db::ATTR_TYPE::unknown:
      {
        throw std::runtime_error("unknown attribute type " + el.dump());
      }
      }
      ++ndx;
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
    if (! flds.empty())
    {
      // r += out::sl(fmt::format("constexpr static const uint max_attr = {};", flds.size()), offs);
      r += out::sl(fmt::format("attr_vec_t A_"), offs);
      r += out::sl(fmt::format("{{"), offs);
      for (const auto& attr : flds) r += out::sl(fmt::format("&{}_,", attr.name()), offs + 2);
      r += out::sl(fmt::format("}};"), offs);
    }
    return r;
  }
  /**
   * @brief define buffer default constructor
   *
   * @param c_name name of the class
   * @param offs   offset from the left border
   * @return str_t generated code
   */
  str_t gen_cpp::define_default_ctor(const fld_vec& /*flds*/,
                                     uint /*max_name_len*/,
                                     cstr_t              c_name,
                                     const db::BUF_TYPE& bt,
                                     std::size_t         offs)
  {
    str_t r;
    auto  BT_name = str_t("db::BUF_TYPE::") + str_t(ME::enum_name<db::BUF_TYPE>(bt));
    r += out::sl(fmt::format("constexpr {0}() : db::buffer_root<{1}, N>()", c_name, BT_name), offs);
    r += out::sl(fmt::format("{{"), offs);
    // auto PP_str = fmt::format("db::buffer_root<{0}, N>", BT_name);
    // r += out::sl(fmt::format("  using PP = typename {};", PP_str), offs);
    // for (const auto& fld : flds)
    // {
    //   auto name = fld.name() + "_";
    //   r += out::sl(fmt::format("PP::attr_vec().emplace_back(&{0:<{1}});", name, max_name_len +
    //   1),
    //                offs + 2);
    // }
    r += out::sl(fmt::format("}}"), offs);
    return r;
  }
  /**
   * @brief define root access to the attributes of the buffer
   *
   * @param offs offset from left border
   * @return str_t generated code
   */
  str_t gen_cpp::define_attribute_access(uint offs)
  {
    str_t r;
    r += out::sl("      attr_vec_t& attributes()       override {return A_;}", offs);
    r += out::sl("const attr_vec_t& attributes() const override {return A_;}", offs);
    return r;
  }
  /**
   * @brief generate buffer
   *
   * @param q        query data
   * @param a_type   buffer type parameter vs result
   * @param offs     offset from the left border
   * @return str_t   generated code
   */
  str_t gen_cpp::gen_buf(const gsql_q& q, const db::BUF_TYPE& bt, uint offs)
  {
    const auto& bd     = q.buf_dscr((bt));
    auto        c_name = snake_case(bd.id());
    auto        bt_str = str_t(ME::enum_name(bt));
    if (bd.should_skip()) return line_text(c_name + "  explicitly skipped", offs);
    if (bd.flds().empty()) return line_text(c_name + str_t(" no ") + bt_str, offs);
    // generating buffers that have one or more par/cols and are not declared as skip
    str_t       r;
    auto        ml   = bd.max_name_len();
    auto        mctl = bd.max_ctype_len();
    const auto& flds = bd.flds();
    auto        txt  = str_t(q.id()) + "::" + str_t(ME::enum_name<db::BUF_TYPE>(bt));
    r += line_text(txt, offs);
    r += out::sl(fmt::format("template <std::size_t N = 1>"), offs);
    auto BT_name = str_t("db::BUF_TYPE::") + str_t(ME::enum_name<db::BUF_TYPE>(bt));
    r += out::sl(fmt::format("class {0} : public db::buffer_root<{1}, N>", c_name, BT_name), offs);
    r += out::sl(fmt::format("{{"), offs);
    r += out::sl(fmt::format("public: /*...public methods...*/"), offs);
    r += out::sl(fmt::format("  using attr_vec_t = std::vector<db::attr_root_root<N>*>;"), offs);
    r += define_default_ctor(flds, ml, c_name, bt, offs + 2);
    r += define_trivial_null_getters(flds, ml, offs + 2);
    r += define_null_getters(flds, ml, offs + 2);
    r += define_trivial_null_setters(flds, ml, mctl, offs + 2);
    r += define_null_setters(flds, ml, mctl, offs + 2);
    r += define_trivial_getters(flds, ml, mctl, offs + 2);
    r += define_getters(flds, ml, mctl, offs + 2);
    r += define_trivial_setters(flds, ml, mctl, offs + 2);
    r += define_setters(flds, ml, mctl, offs + 2);
    r += out::sl(fmt::format("protected:/*...protected methods..............*/"), offs);
    r += define_attribute_access(offs + 2);
    r += out::sl(fmt::format("private:  /*...private methods & attributes...*/"), offs);
    r += define_ID_const(flds, ml, offs + 2);
    r += define_attributes_const(flds, ml, offs + 2);
    r += define_attr_types(flds, ml, offs + 2);
    r += define_attributes(flds, ml, offs + 2);
    r += define_attr_array(bd.flds(), ml, offs + 2);
    r += out::sl(fmt::format("}}; // class {}", c_name), offs);
    return r;
  }
  /**
   * @brief generates code fragment to declare the SQL statement
   *
   * @param q
   * @param offs
   * @return str_t
   */
  str_t gen_cpp::gen_const_sql(const gsql_q& q, uint offs)
  {
    str_t       r;
    const auto& ml_sql = q.sql_set().get().sql_ml().lines();
    r += out::sl(fmt::format("constexpr static const cstr_t sql_ ="), offs);
    r += out::sl(fmt::format("R\"k0a1f2k3a4("), offs);
    for (auto l : ml_sql) { r += out::sl(fmt::format("{}", l), offs + 2); }
    r += out::sl(fmt::format(")k0a1f2k3a4\";"), offs);
    return r;
  }
  /**
   * @brief build utl template header
   *
   * depending of on the parameter combination it generates:
   * - no param necessary ""
   * - par is necessary "template<std::size_t N_par = 1>
   * - res is necessary "template<std::size_t N_res = 1>
   * - both are necessary "template<std::size_t N_par = 1, std::size_t N_res = 1>
   * @param q q description
   * @return str_t generated code
   */
  str_t gen_cpp::gen_templ_hdr(const gsql_q& q)
  {
    str_t r;
    for (const auto& b : q.buf())
    {
      if (b.is_required())
        r += str_t("std::size_t N_") +                      //
             str_t(ME::enum_name<db::BUF_TYPE>(b.type())) + //
             " = 1, ";
    }
    if (! r.empty()) r.resize(r.size() - 2); // if no res && par => nothin tu cut away
    if (r.empty()) return "";
    r = "template<" + r + ">";
    return r;
  }
  str_t gen_cpp::gen_utl_ctor(const gsql_q& q, uint offs)
  {
    str_t r;
    r += out::sl(fmt::format("explicit utl(db::connection* c)"), offs);
    r += out::sl(fmt::format(": db::utl(c, sql_)"), offs);
    r += out::sl(fmt::format("{{"), offs);
    for (const auto& b : q.buf())
    {
      if (b.is_required())
        r += out::sl(fmt::format("  set_{0}_buf(&{0}_);", ME::enum_name<db::BUF_TYPE>(b.type())),
                     offs);
    }
    r += out::sl(fmt::format("}}"), offs);
    return r;
  }
  str_t gen_cpp::gen_utl_getters(const gsql_q& q, uint offs)
  {
    str_t r;
    for (auto bt : ME::enum_values<db::BUF_TYPE>())
    {
      const auto& buf = q.buf_dscr(bt);
      if (buf.is_required())
      {
        r +=             //
          out::sl(       //
            fmt::format( //
              "{0}<N_{1}>* {1}_buf() override "
              "{{"
              "return dynamic_cast<{2}<N_{1}>*>(db::utl::{1}_buf());"
              "}}",
              buf.class_name(),
              ME::enum_name<db::BUF_TYPE>(bt),
              buf.class_name()),
            offs);
        //            dynamic_cast<par<N_par>&>(db::utl::par_buf());
      }
    };
    return r;
  }
  str_t gen_cpp::gen_utl_member_var(const gsql_q& q, uint offs)
  {
    str_t r;
    for (auto bt : ME::enum_values<db::BUF_TYPE>())
    {
      const auto& buf = q.buf_dscr(bt);
      if (buf.is_required())
      {
        r += out::sl(fmt::format( //
                       "  {0}<N_{1}> {1}_{{}};",
                       buf.class_name(),
                       ME::enum_name<db::BUF_TYPE>(bt)),
                     offs);
      }
    };
    return r;
  }
  /**
   * @brief generate utility class
   *
   * @param q
   * @param offs
   * @return str_t
   */
  str_t gen_cpp::gen_utl(const gsql_q& q, uint offs)
  {
    str_t       r;
    auto        q_name  = q.namespace_str();
    auto        sql     = q.sql();
    const auto* cl_name = "utl";
    auto        txt     = fmt::format("{0}::{1}", q_name, cl_name);
    r += line_text(txt, offs);
    r += out::sl(fmt::format("{}", gen_templ_hdr(q)), offs);
    r += out::sl(fmt::format("class {}: public db::utl", cl_name), offs);
    r += out::sl(fmt::format("{{"), offs);
    r += out::sl(fmt::format("public:"), offs);
    r += out::sl(fmt::format("  using brr = db::buffer_root_root;"), offs);
    r += gen_utl_ctor(q, offs + 2);
    r += gen_utl_getters(q, offs + 2);
    r += define_allowed_codes(q.allowed(), offs + 2);
    r += out::sl(fmt::format("private:"), offs);
    r += gen_utl_member_var(q, offs + 2);
    r += gen_const_sql(q, offs + 2);
    r += out::sl(fmt::format("}};"), offs);

    return r;
  }
  str_t gen_cpp::gen_includes(uint offs)
  {
    str_t r;
    r += out::sl("#include <array>", offs);
    r += out::sl("#include <span>", offs);
    r += out::sl("#include <string>", offs);
    r += out::sl("#include <string_view>", offs);
    r += out::sl("", offs);
    r += out::sl(fmt::format("#include \"{}\"", "dbgen3_templ.hpp"), offs);
    return r;
  }
  /**
   * @brief generate code for one query
   *
   * @param q data structure of the query we want generate code for
   * @param offs offset from left border
   * @return str_t generated code
   *
   * Code is generated only if there there exist actual sql command within the
   * query definition.
   */
  str_t gen_cpp::gen_query(const gsql_q& q, uint offs)
  {
    str_t r;
    auto  ns  = snake_case(q.id());
    auto  sql = q.sql();
    if (! sql.empty())
    {
      r += line_text(ns, offs);
      r += out::sl(fmt::format("namespace {}", ns), offs);
      r += out::sl(fmt::format("{{"), offs);
      for (auto bt : ME::enum_values<db::BUF_TYPE>()) r += gen_buf(q, bt, offs + 2);
      r += gen_utl(q, offs + 2);
      r += out::sl(fmt::format("}}; //namespace {}", ns), offs);
    }
    else //
    {
      auto msg =
        fmt::format(" There is no valid SQL statement in query '{}'. No code is generated. ", ns);
      r += line_text(msg, offs, '!');
    }
    return r;
  };
  /**
   * @brief generate set of queries
   *
   * @param set structure with definition of queries
   * @param offs  offset from left border
   * @return str_t generated code
   */
  str_t gen_cpp::gen_queries(const gsql_q_set& set, uint offs)
  {
    str_t r;
    for (const auto& q : set.q_vec()) { r += gen_query(q, offs + 2); };
    return r;
  };

  str_t gen_cpp::gen_custom_header(const gsql_q_set& set, uint offs)
  {
    str_t r;
    auto  h_ml = set.header_multi_line().lines();
    for (const auto& line : h_ml) r += out::sl(line, offs);
    return r;
  }

  str_t gen_cpp::gen_file(uint offs)
  {
    constexpr static const uint ctime_buf_len = 26;

    using ctime_buf_t = std::array<char, ctime_buf_len>;
    str_t       r;
    auto        now  = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    ctime_buf_t buf{};
    std::string time_str = ctime_r(&time, buf.data());

    auto ns = set().namespace_str();
    r += out::sl(fmt::format( //
                   "/* auto generated '{}' - do not alter */",
                   time_str.substr(0, time_str.size() - 1)),
                 offs);
    r += out::sl(fmt::format("#ifndef gen_{}_hpp", ns));
    r += out::sl(fmt::format("#define gen_{}_hpp", ns));
    r += gen_includes(offs);
    r += gen_custom_header(set(), offs);
    r += out::sl(fmt::format("namespace {}", ns), offs);
    r += out::sl("{", offs);
    r += out::sl("  using uint    = std::size_t;", offs);
    r += out::sl("  using str_t   = std::string;", offs);
    r += out::sl("  using cstr_t  = std::string_view;", offs);
    r += out::sl("  using bstr_t  = std::span<uint8_t>;", offs);
    r += out::sl("  using cbstr_t = std::span<const uint8_t>;", offs);
    r += gen_queries(set(), offs);
    r += out::sl(fmt::format("}}; //namespace {}", ns), offs);
    r += out::sl(fmt::format("#endif // gen_{}_hpp", ns));
    return r;
  }

}; // namespace dbgen3