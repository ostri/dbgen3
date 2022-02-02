#ifndef GEN_HPP
#define GEN_HPP

#include <string>

#include "gsql_q_set.hpp"

namespace dbgen3
{
  using str_t = std::string;


  class gen
  {
  public:
    gen() = default;
    virtual ~gen(){};
    virtual str_t gen_file(uint offs) = 0;
    void          set_set(const gsql_q_set& a_set) { set_ = &a_set; }
    void          set_rdbm(const RDBMS& rdbm) { this->rdbm_ = rdbm; }
  protected:
    const gsql_q_set& set() const;
    RDBMS             rdbm() const { return this->rdbm_; }
    static str_t      line(uint a_len, uint offs = 0, char ch = '.');
    static str_t      snake_case(cstr_t a_name);
  private:
    const gsql_q_set* set_ = nullptr; //!< gsql set data
    RDBMS             rdbm_;          //!< database type we are generating code for
  };
}; // namespace dbgen3

#endif // GEN_HPP
