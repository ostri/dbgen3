#include <sstream>

#include "runtime/error_dscr.hpp"

namespace db
{
  error_dscr::error_dscr(int code, cstr_t state, cstr_t msg)
  : code_(code)
  , state_(state)
  , msg_(msg)
  { }
  str_t error_dscr::dump(cstr_t a_msg) const
  {
    return str_t(a_msg) + " [code: " + std::to_string(code_) + " state: '" + str_t(state_) + "' msg: '" + msg_ +
         "']";
  }
} // namespace db
