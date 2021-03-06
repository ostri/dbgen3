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
    static str_t gen_templ_hdr(const gsql_q& q);
    static str_t gen_utl_ctor(const gsql_q& q, uint offs);
    static str_t gen_utl_getters(const gsql_q& q, uint offs);
    static str_t gen_utl_member_var(const gsql_q& q, uint offs);
    static str_t gen_buf(const gsql_q& q, const db::BUF_TYPE& bt, uint offs);
    static str_t gen_utl(const gsql_q& q, uint offs);

    static str_t define_trivial_null_getters(const fld_vec& flds, uint max_name_len, uint offs);
    static str_t define_trivial_getters(const fld_vec& flds,
                                        uint           max_name_len,
                                        uint           max_ctype_len,
                                        uint           offs);
    static str_t define_null_getters(const fld_vec& flds, uint max_name_len, uint offs);
    static str_t define_getters(const fld_vec& flds,
                                uint           max_name_len,
                                uint           max_ctype_len,
                                uint           offs);
    static str_t define_trivial_null_setters(const fld_vec& flds,
                                             uint           max_name_len,
                                             uint           max_ctype_len,
                                             uint           offs);
    static str_t define_trivial_setters(const fld_vec& flds,
                                        uint           max_name_len,
                                        uint           max_ctype_len,
                                        uint           offs);
    static str_t define_null_setters(const fld_vec& flds,
                                     uint           max_name_len,
                                     uint           max_ctype_len,
                                     uint           offs);
    static str_t define_setters(const fld_vec& flds,
                                uint           max_name_len,
                                uint           max_ctype_len,
                                uint           offs);
    static str_t define_allowed_codes(const int_vec& allowed_codes, uint offs);
    static str_t define_dump(const fld_vec& flds, uint max_name_len, uint offs);
//    static str_t define_ID_const(const fld_vec& flds, uint /*max_name_len*/, uint offs);
    static str_t define_attributes_const(const fld_vec& flds, uint max_name_len, uint offs);
    static str_t define_attr_types(const fld_vec& flds, uint max_type_len, uint offs);

    static str_t define_attributes(const fld_vec& flds, uint max_name_len, uint offs);
    static str_t define_attribute_access(uint offs);
    static str_t define_attr_array(const fld_vec& flds, uint max_width, uint offs);
    static str_t define_attr_name_array(const fld_vec& flds, uint max_width, uint offs);

    static str_t gen_includes(uint offs);
    static str_t gen_queries(const gsql_q_set& set, uint offs);
    static str_t gen_custom_header(const gsql_q_set& set, uint offs);
    static str_t gen_const_sql(const gsql_q& q, uint offs);
    static str_t gen_query(const gsql_q& q, uint offs);
    static str_t define_default_ctor(const fld_vec&      flds,
                                     uint                max_name_len,
                                     cstr_t              c_name,
                                     const db::BUF_TYPE& bt,
                                     std::size_t         offs);
  };
}; // namespace dbgen3

#endif // GEN_CPP_HPP
