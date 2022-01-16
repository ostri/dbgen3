#include "program_status.hpp"
#include "enums.hpp"
namespace dbgen3
{

  const std::string& program_status::dscr(const P_STS& code) const
  {
    assert(static_cast<uint>(code) < dic_.size()); // NOLINT
    uint ndx = ME::enum_integer<P_STS>(code);
    return dic_.at(ndx).dscr;
  }
} // namespace dbgen3
