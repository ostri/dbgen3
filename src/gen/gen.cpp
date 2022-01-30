
#include "gen.hpp"
#include "string_format.hpp"

namespace dbgen3
{

  const gsql_q_set& gen::set() const { return *set_; }

  str_t gen::line(uint a_len, uint offs, char ch)
  {
      str_t r = "/* " + std::string(a_len, ch) + " */";
      return out::sl(r, offs);
  }

  str_t gen::snake_case(cstr_t a_name)
  {
      str_t r;
      for (auto ch: a_name)
      if (ch == '-') r += '_'; else r += ch;
      return r;
  }


}; // namespace dbgen3