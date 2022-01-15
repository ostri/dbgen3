#ifndef GSQL_Q_SET_HPP
#define GSQL_Q_SET_HPP

#include <map>

#include "gsql_q.hpp"
#include "multi_line.hpp"

namespace dbgen3
{
  class gsql_q_set
  {
  public:
    using q_dic                   = std::map<std::string, gsql_q>;
    gsql_q_set()                  = default;
    ~gsql_q_set()                 = default;
    gsql_q_set(const gsql_q_set&) = delete;
    gsql_q_set(gsql_q_set&&)      = default;
    gsql_q_set& operator=(const gsql_q_set&) = default;
    gsql_q_set& operator=(gsql_q_set&&) = default;
    /// @name getters
    //@{
    std::string       header_str(int offs) const;
    const multi_line& header_multi_line() const;
    std::string       dump() const;
    std::string       dump(int offs) const;
    std::string       dump(const std::string& a_msg, int offs) const;
    std::string       id() const { return this->id_; }
    //@}
    /// @name setters
    //@{
    void set_header(const std::string& header_str);
    void set_id(const std::string& id);
    bool q_insert(const gsql_q& q)
    {
      const auto [key, sts] = q_dic_.emplace(q.id(), q);
      err << "key" << key->first ;//<< " status " << sts << std::endl;
      return sts;
    }
    //@}
  private:
    multi_line  header_; //!< contents of the header of query set (q-set)
    std::string id_;     //!< q_set unique id; if not provided the basename of gsql filename
    q_dic       q_dic_;  //!< dictionary of queries
  };


  /// fetch header as a string
  inline std::string gsql_q_set::header_str(int offs) const { return header_.dump("", offs); }
  /// fetch header as multiline vector
  inline const multi_line& gsql_q_set::header_multi_line() const { return header_; }
  /// set id of the query set
  inline void gsql_q_set::set_id(const std::string& id) { this->id_ = id; }
}; // namespace dbgen3

#endif // GSQL_Q_SET_HPP
