#include <sstream>
#include "error_dscr.hpp"

namespace db
{
  error_dscr::error_dscr(int code, const std::string& state, const std::string& msg)
  : code_(code)
  , state_(std::move(state))
  , msg_(std::move(msg))
  {
  }
  std::string        error_dscr::dump(const std::string& a_msg) const
  {
    std::stringstream s;
    s << a_msg << " [code: " << code_ << " state: " << state_ << " msg: " << msg_ << "]";
    //log(s.str());
    return s.str();
  }
} // namespace dbgen3
