#include "string_format.hpp"
#include "gsql_q_set.hpp"

namespace dbgen3
{

  std::string gsql_q_set::dump() const { return dump("", 0); }
  std::string gsql_q_set::dump(int offs) const { return dump("", offs); }
  std::string gsql_q_set::dump(const std::string& a_msg, int offs) const
  {
    std::string s;
    if (! a_msg.empty()) s += out::sl(a_msg, offs);
    s += out::sl("query-set:", offs);
    s += out::sl("{", offs);
    s += out::sl("  id: '" + id_ + "'", offs);
    s += out::sl("  header:", offs);
    s += out::sl("  {", offs);
    s += header_.dump("", offs + 4);
    s += out::sl("  }", offs);
    for (auto const& [key, val] : q_dic_) { s += val.dump(offs + 2); }
    s += out::sl("}", offs);
    return s;
  };

  //@}
  /// @name setters
  //@{
  void gsql_q_set::set_header(const std::string& header_str) { header_ = header_str; }

}; // namespace dbgen3
