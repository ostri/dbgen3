#ifndef GSQL_Q_HPP
#define GSQL_Q_HPP

#include <array>
#include <utility>

#include "utility_classes/common.hpp"
#include "utility_classes/enums.hpp"
#include "gsql_parser/gsql_qbuf_dscr.hpp"
#include "gsql_parser/gsql_sql_set.hpp"
#include "magic_enum.hpp"
#include "runtime/odbc_db.hpp"


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
    q_buf_arr&            buf_dscr();
    const gsql_qbuf_dscr& buf_dscr(const db::BUF_TYPE& a_type) const;
    q_buf_arr&            buf();
    const q_buf_arr&      buf() const;
    std::string sql() const;
    std::string sql_prep() const;
    RDBMS       db_type() const;
    str_t       namespace_str() const;
    std::string dump() const;
    std::string dump(uint offs) const;
    std::string dump(const std::string& a_msg, uint offs) const;
    bool must_have_buf(const db::BUF_TYPE& a_type) const;

    //@}
    /// @name setters
    //@{
    void set_id(cstr_t o);
    void set_buf_dscr(const gsql_qbuf_dscr& buf_dscr);
    void set_sql_set(const gsql_sql_set& o);
    void set_buf_dscr_flds(const db::BUF_TYPE& a_bt, const fld_vec& a_fld_vec);
    //@}
    const gsql_sql_set& sql_set() const; //<! fetches vector of sql descriptions
    const int_vec&      allowed() const;
    void                set_allowed(const int_vec& allowed);
    void                set_allowed(cstr_t allowed);
  private:
    //    uint size() const; //!< number of sql statements
    /*...................................................................*/
    str_t        id_{};       //!< name of the gsql_id (unique id)
    q_buf_arr    buf_dscr_{}; //!< query buffer descriptions
    gsql_sql_set sql_set_;    //!< set of sql statements in the gsql item FIXME only buf_dscr
    int_vec      allowed_{};  //!< allowed errorcodes i.e. they dont throw (0 and others)
  };

}; // namespace dbgen3

#endif // GSQL_Q_HPP
