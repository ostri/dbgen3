#ifndef GEN_HPP
#define GEN_HPP

#include <string>

#include "enums.hpp"
#include "gsql_q_set.hpp"

namespace dbgen3
{
  using str_t = std::string;


  class gen
  {
  public:
    gen()                 = default;
    virtual ~gen()        = default;
    gen(const gen&)       = default;
    gen(gen&&)            = default;
    gen&          operator=(const gen&) = default;
    gen&          operator=(gen&&)    = default;
    virtual str_t gen_file(uint offs) = 0;
    void          set_set(const gsql_q_set& a_set);
    void          set_rdbm(const RDBMS& rdbm);
  protected:
    const gsql_q_set& set() const;
    RDBMS             rdbm() const;
    static str_t      line(uint offs = 0, char ch = '.');
    static str_t      line_text(cstr_t a_msg, uint offs = 0, char ch = '.');
  private:
    static const int  generated_width = 100;     //!< width of the generated document (well mostly)
    const gsql_q_set* set_            = nullptr; //!< gsql set data
    RDBMS             rdbm_           = RDBMS::db2; //!< database type we are generating code for
  };
}; // namespace dbgen3

#endif // GEN_HPP
