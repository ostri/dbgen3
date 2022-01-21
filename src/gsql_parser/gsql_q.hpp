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
    const gsql_qbuf_dscr& buf_dscr(const BUF_TYPE& a_type) const;
    const std::string     sql(const RDBMS& a_db, const PHASE& a_phase) const;
    std::string           dump() const;
    std::string           dump(uint offs) const;
    std::string           dump(const std::string& a_msg, uint offs) const;

    //@}
    /// @name setters
    //@{
    void set_id(const str_t& o);
    void set_buf_dscr(const gsql_qbuf_dscr& buf_dscr);
    void set_sql_set(const gsql_sql_set& o);
    void set_buf_dscr_flds(const BUF_TYPE& a_bt, const fld_vec& a_fld_vec)
    {
      buf_dscr_[ME::enum_integer<BUF_TYPE>(a_bt)].set_flds(a_fld_vec);
    }
    //@}
  private:
    const gsql_sql_set& sql_set() const; //<! fetches vector of sql descriptions
    uint                size() const;    //!< number of sql statements
    /*...................................................................*/
    str_t        id_;       //!< name of the gsql_id (unique id)
    q_buf_arr    buf_dscr_; //!< query buffer descriptions
    gsql_sql_set sql_set_;  //!< set of sql statements in the gsql item
  };

  inline cstr_t            gsql_q::id() const { return id_; }
  inline const std::string gsql_q::sql(const RDBMS& a_db, const PHASE& a_phase) const
  {
    return sql_set_.smart_fetch_sql(a_db, a_phase);
  }
  inline const gsql_sql_set& gsql_q::sql_set() const { return sql_set_; }
  inline uint                gsql_q::size() const { return sql_set_.size(); }

  inline void gsql_q::set_id(const str_t& o) { id_ = o; }

  inline void gsql_q::set_sql_set(const gsql_sql_set& o) { sql_set_ = o; }
}; // namespace dbgen3

#endif // GSQL_Q_HPP
