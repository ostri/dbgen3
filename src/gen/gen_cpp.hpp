#ifndef GEN_CPP_HPP
#define GEN_CPP_HPP

#include "gen.hpp"

namespace dbgen3
{
  class gen_cpp : public gen
  {
  public:
    gen_cpp()          = default;
    virtual ~gen_cpp() = default;
    virtual str_t gen_file(uint offs) override;
  private:
    static str_t gen_buf(const gsql_q& q, const BUF_TYPE& a_type, uint offs);
    static str_t gen_utl(const gsql_q& q, uint offs);

  protected:
    static str_t define_attributes(const fld_vec& flds, uint offs);
  };
}; // namespace dbgen3

#endif // GEN_CPP_HPP
