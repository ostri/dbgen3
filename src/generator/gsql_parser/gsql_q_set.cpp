#include "utility_classes/string_format.hpp"
#include "gsql_parser/gsql_q_set.hpp"

namespace dbgen3
{

  str_t             gsql_q_set::namespace_str() const { return snake_case(id_); }

  gsql_q&        gsql_q_set::q_vec(std::size_t ndx) { return q_vec_.at(ndx); }

  const gsql_q&  gsql_q_set::q_vec(std::size_t ndx) const { return q_vec_.at(ndx); }

  q_vec_t&       gsql_q_set::q_vec() { return q_vec_; }

  const q_vec_t& gsql_q_set::q_vec() const { return q_vec_; }

  void gsql_q_set::set_header(const std::string& header_str) { header_ = header_str; }

}; // namespace dbgen3
