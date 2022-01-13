#ifndef GSQL_ITEM_SET_HPP
#define GSQL_ITEM_SET_HPP

#include <map>

#include "gsql_q.hpp"

namespace dbgen3
{
  class gsql_q_set : public std::map<str_t, gsql_q>
  {
  public:
    gsql_q_set()                     = default;
    ~gsql_q_set()                    = default;
    gsql_q_set(const gsql_q_set&) = default;
    gsql_q_set(gsql_q_set&&)      = default;
    gsql_q_set& operator=(const gsql_q_set&) = default;
    gsql_q_set& operator=(gsql_q_set&&) = default;

    const std::string& header() const { return this->header_; }
    void set_header(const std::string &header) { this->header_ = header; }

  private:
    std::string header_; //!< contents of the header of query set (q-set)
  };
}; // namespace dbgen3

#endif // GSQL_ITEM_SET_HPP
