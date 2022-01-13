#include "program_status.hpp"

namespace dbgen3
{

  const std::string& program_status::g_dscr(p_sts code) const
  {
    assert(static_cast<uint>(code) >= 0);          // NOLINT
    assert(static_cast<uint>(code) < dic_.size()); // NOLINT
    return dic_.at(static_cast<uint>(code)).dscr;
  }
} // namespace dbgen3
