#ifndef GSQL_Q_SET_HPP
#define GSQL_Q_SET_HPP

#include <map>

#include "gsql_parser/gsql_q.hpp"
#include "utility_classes/multi_line.hpp"

namespace dbgen3
{
    using q_dic_t = std::map<std::string, int>; // string q.ID() int index in vector
    using q_vec_t = std::vector<gsql_q>;
  class gsql_q_set
  {
  public:
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
    str_t             dump() const;
    str_t             dump(int offs) const;
    str_t             id() const;
    str_t             namespace_str() const;
    gsql_q&           q_vec(std::size_t ndx);
    const gsql_q&     q_vec(std::size_t ndx) const;
    q_vec_t&          q_vec();
    const q_vec_t&    q_vec() const;
    //@}
    /// @name setters
    //@{
    void        set_header(const std::string& header_str);
    void        set_id(const std::string& id);
    bool        q_insert(const gsql_q& q);
    std::string dump(cstr_t msg) const;
    std::string dump(cstr_t a_msg, int offs) const;
    //@}
  private:
    multi_line  header_; //!< contents of the header of query set (q-set)
    std::string id_;     //!< q_set unique id; if not provided the basename of gsql filename
    q_dic_t     q_dic_;  //!< dictionary of query ids
    q_vec_t     q_vec_;  //!< vector of all query definitions
  };


  /// fetch header as a string
  inline std::string gsql_q_set::header_str(int offs) const { return header_.dump("", offs); }
  /// fetch header as multiline vector
  inline const multi_line& gsql_q_set::header_multi_line() const { return header_; }

  inline std::string gsql_q_set::dump() const { return dump("", 0); }

  inline std::string gsql_q_set::dump(int offs) const { return dump("", offs); }

  inline std::string gsql_q_set::id() const { return this->id_; }

  /// set id of the query set
  inline void gsql_q_set::set_id(const std::string& id) { this->id_ = id; }

  inline bool gsql_q_set::q_insert(const gsql_q& q)
  {
    q_vec_.emplace_back(q);
    const auto [key, sts] = q_dic_.emplace(q.id(), q_vec_.size() - 1);
    return sts;
  }

  inline std::string gsql_q_set::dump(cstr_t msg) const { return dump(msg, 0); }

  inline std::string gsql_q_set::dump(cstr_t a_msg, int offs) const
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
    for (auto const& val : q_vec_) { s += val.dump(offs + 2); }
    s += out::sl("}", offs);
    return s;
  };

}; // namespace dbgen3

#endif // GSQL_Q_SET_HPP
