#ifndef GEN_CPP_HPP
#define GEN_CPP_HPP

#include "gen.hpp"

namespace dbgen3
{
  class gen_cpp : public gen
  {
  public:
    gen_cpp()               = default;
    ~gen_cpp() override     = default;
    gen_cpp(const gen_cpp&) = default;
    gen_cpp(gen_cpp&&)      = default;
    gen_cpp& operator=(const gen_cpp&) = default;
    gen_cpp& operator=(gen_cpp&&) = default;
    str_t    gen_file(uint offs) override;
  private:
    static str_t gen_buf(const gsql_q& q, const db::BUF_TYPE& bt, uint offs);
    static str_t gen_utl(const gsql_q& q, uint offs);

    static str_t define_trivial_getters(const fld_vec& flds, uint max_name_len, uint offs);
    static str_t define_getters(const fld_vec& flds, uint max_name_len, uint offs);
    static str_t define_trivial_setters(const fld_vec& flds, uint max_name_len, uint offs);
    static str_t define_setters(const fld_vec& flds, uint max_name_len, uint offs);
    static str_t define_dump(const fld_vec& flds, uint max_name_len, uint offs);
    static str_t define_attributes_const(const fld_vec& flds, uint max_name_len, uint offs);
    static str_t define_attributes(const fld_vec& flds, uint max_name_len, uint offs);
    static str_t define_attr_array(const fld_vec& flds, uint max_name_len, uint offs);

    static str_t gen_includes(uint offs);
    static str_t gen_queries(const gsql_q_set& set, uint offs);
    static str_t gen_custom_header(const gsql_q_set& set, uint offs);
    static str_t gen_const_sql(const gsql_q& q, uint offs);
    static str_t gen_query(const gsql_q& q, uint offs);
    static str_t define_default_ctor(const fld_vec& flds,
                                     uint           max_name_len,
                                     cstr_t         c_name,
                                     const db::BUF_TYPE& bt,
                                     std::size_t    offs);

  };
}; // namespace dbgen3

#endif // GEN_CPP_HPP
