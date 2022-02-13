#ifndef GSQL_Q_HPP
#define GSQL_Q_HPP

#include <array>
#include <utility>

#include "common.hpp"
#include "enums.hpp"
#include "gsql_qbuf_dscr.hpp"
#include "gsql_sql_set.hpp"
#include "magic_enum.hpp"
#include "odbc_db.hpp"


namespace dbgen3
{
  /**
   * @brief one SQL statement description within the gsql file
   *
   */
  // using sql_arr   = std::array<str_t, 3>;
  using q_buf_arr = std::array<gsql_qbuf_dscr, ME::enum_count<db::BUF_TYPE>()>;

  class gsql_q
  {
    /**
     * placeholder for sql statements
     * 1 - main sql statement
     * 0,2 needed for proper dbgen3 execution (e.g. work with the temporary tables)
     */
  public:
    explicit gsql_q(const RDBMS& a_db_type);
    ~gsql_q()             = default;
    gsql_q(const gsql_q&) = default;
    gsql_q(gsql_q&&)      = default;
    gsql_q& operator=(const gsql_q&) = default;
    gsql_q& operator=(gsql_q&&) = default;
    ///@}
    /// @name getters
    ///@{
    cstr_t                id() const;
    q_buf_arr&            buf_dscr() { return buf_dscr_; }
    const gsql_qbuf_dscr& buf_dscr(const db::BUF_TYPE& a_type) const;
    q_buf_arr&            buf() { return buf_dscr_; }
    const q_buf_arr&      buf() const { return buf_dscr_; }
    std::string           sql(const PHASE& a_phase) const;
    std::string           sql() const { return sql(PHASE::main); };
    RDBMS                 db_type() const;
    str_t                 namespace_str() const;
    std::string           dump() const;
    std::string           dump(uint offs) const;
    std::string           dump(const std::string& a_msg, uint offs) const;
    /**
     * @brief does SQL demands a buffer (PAR/RES)
     *
     * @param a_type type of buffer (PAR/RES)
     * @return true SQL requires the buffer
     * @return false SQL does not mandate the buffer
     */
    bool must_have_buf(const db::BUF_TYPE& a_type) const
    {
      return ! buf_dscr(a_type).must_generate();
    }

    //@}
    /// @name setters
    //@{
    void set_id(cstr_t o);
    void set_buf_dscr(const gsql_qbuf_dscr& buf_dscr);
    void set_sql_set(const gsql_sql_set& o);
    void set_buf_dscr_flds(const db::BUF_TYPE& a_bt, const fld_vec& a_fld_vec);
    //@}
    const gsql_sql_set& sql_set() const; //<! fetches vector of sql descriptions
  private:
    uint size() const; //!< number of sql statements
    /*...................................................................*/
    str_t        id_{};       //!< name of the gsql_id (unique id)
    q_buf_arr    buf_dscr_{}; //!< query buffer descriptions
    gsql_sql_set sql_set_;    //!< set of sql statements in the gsql item
  };

}; // namespace dbgen3

#endif // GSQL_Q_HPP
