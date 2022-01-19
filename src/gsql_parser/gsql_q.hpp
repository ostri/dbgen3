#ifndef GSQL_Q_HPP
#define GSQL_Q_HPP

#include <array>
#include <utility>

#include "common.hpp"
#include "gsql_qbuf_dscr.hpp"
#include "gsql_sql_set.hpp"


namespace dbgen3
{
  /**
   * @brief one SQL statement description within the gsql file
   *
   */
  using sql_arr   = std::array<str_t, 3>;
  using q_buf_arr = std::array<gsql_qbuf_dscr, 2>;

  class gsql_q
  {
    /**
     * placeholder for sql statements
     * 1 - main sql statement
     * 0,2 needed for proper dbgen3 execution (e.g. work with the temporary tables)
     */
  public:
    gsql_q()              = default;
    ~gsql_q()             = default;
    gsql_q(const gsql_q&) = default;
    gsql_q(gsql_q&&)      = default;
    gsql_q& operator=(const gsql_q&) = default;
    gsql_q& operator=(gsql_q&&) = default;
    ///@}
    /// @name getters
    ///@{
    cstr_t                id() const;
    const gsql_sql_set&   sql_set() const;
    cstr_t                sql(uint ndx) const;
    uint                  size() const;
    std::string           dump() const;
    std::string           dump(uint offs) const;
    std::string           dump(const std::string& a_msg, uint offs) const;
    const gsql_qbuf_dscr& buf_dscr(const BUF_TYPE& a_type) const;
    std::string           fetch_sql(const RDBMS& a_db, const PHASE& a_phase) const
    {
      gsql_sql_set::iterator it = sql_set_.smart_fetch(a_db, a_phase);
//      if (it != sql_set_.end()) return it->second.
    }

    //@}
    /// @name setters
    //@{
    void set_id(const str_t& o);
    void s_sql(uint ndx, const str_t& sql);
    void set_buf_dscr(const gsql_qbuf_dscr& buf_dscr);
    void set_sql_set(const gsql_sql_set& o);
    //@}
  private:
    str_t        id_;       //!< name of the gsql_id (unique id)
    q_buf_arr    buf_dscr_; //!< query buffer descriptions
    gsql_sql_set sql_set_;  //!< set of sql statements in the gsql item
  };

  inline cstr_t              gsql_q::id() const { return id_; }
  inline const gsql_sql_set& gsql_q::sql_set() const { return sql_set_; }
  inline uint                gsql_q::size() const { return sql_set_.size(); }

  inline void gsql_q::set_id(const str_t& o) { id_ = o; }

  inline void gsql_q::set_sql_set(const gsql_sql_set& o) { sql_set_ = o; }
}; // namespace dbgen3

#endif // GSQL_Q_HPP
