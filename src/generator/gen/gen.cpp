#include <array>
#include <sqlcli1.h>

#include "gen/gen.hpp"
#include "utility_classes/enums.hpp"
#include "utility_classes/string_format.hpp"

namespace dbgen3
{

  void              gen::set_set(const gsql_q_set& a_set) { set_ = &a_set; }
  void              gen::set_rdbm(const RDBMS& rdbm) { this->rdbm_ = rdbm; }
  const gsql_q_set& gen::set() const { return *set_; }
  RDBMS             gen::rdbm() const { return this->rdbm_; }

  /**
   * @brief the same as line_text only without the text
   *
   * @param a_len
   * @param offs
   * @param ch
   * @return str_t
   */
  str_t gen::line(uint offs, char ch) { return line_text("", offs, ch = '.'); }
  /**
   * @brief it writes / *....text.........* /
   *
   * @param a_msg message to be displayed
   * @param offs offset from left
   * @param ch  fill character
   * @return str_t generated text
   */
  str_t gen::line_text(cstr_t a_msg, uint offs, char ch)
  {
    str_t prefix  = str_t(4, ch);
    auto  act_len = generated_width - a_msg.size() - prefix.size() - offs;
    str_t r(str_t("/*") + prefix + str_t(a_msg) + str_t(act_len, ch) + str_t("*/"));
    return out::sl(r, offs);
  }


}; // namespace dbgen3