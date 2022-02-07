#include "error_dscr.hpp"
#include <sstream>

namespace db
{
  error_dscr::error_dscr(int code, cstr_t state, cstr_t msg)
  : code_(code)
  , state_(state)
  , msg_(msg)
  { }
  str_t error_dscr::dump(cstr_t a_msg) const
  {
    str_t s;
    s += str_t(a_msg) + " [code: " + std::to_string(code_) + " state: " + state_ + " msg: " + msg_ +
         "]";
    // log(s.str());
    return s;
  }
} // namespace db
