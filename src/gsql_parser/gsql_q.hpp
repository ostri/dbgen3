#ifndef GSQL_Q_HPP
#define GSQL_Q_HPP

#include <array>
#include <utility>

#include "common.hpp"
#include "gsql_qbuf_dscr.hpp"


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
    gsql_q() = default;
    gsql_q(const str_t& core_name, sql_arr sql);
    ~gsql_q()              = default;
    gsql_q(const gsql_q&)  = default;
    gsql_q(gsql_q&&)       = default;
    gsql_q&        operator=(const gsql_q&) = default;
    gsql_q&        operator=(gsql_q&&) = default;
    cstr_t         id() const;
    const sql_arr& g_sql() const;
    cstr_t         sql(uint ndx) const;
    std::string    dump() const;
    std::string    dump(uint offs) const;
    std::string    dump(const std::string& a_msg, uint offs) const;
    ///@}
    /// @name setters
    ///@{
    void                  set_id(const str_t& o) { id_ = o; }
    void                  s_sql(const sql_arr& sql) { sql_ = sql; }
    void                  s_sql(uint ndx, const str_t& sql);
    const gsql_qbuf_dscr& buf_dscr(enum gsql_qbuf_dscr::type ndx) const;
    // void                  set_buf_dscr(const gsql_qbuf_dscr& buf_dscr, uint ndx);
    void set_buf_dscr(const gsql_qbuf_dscr& buf_dscr);
    ///@}
  private:
    str_t     id_;       //!< name of the gsql_id (unique id)
    q_buf_arr buf_dscr_; //!< query buffer descriptions
    sql_arr   sql_;      //!< set of sql statements in the gsql item
  };

  inline gsql_q::gsql_q(const str_t& core_name, sql_arr sql)
  : id_(core_name)
  , sql_(std::move(sql))
  { }

  /// @name getters
  ///@{
  inline cstr_t         gsql_q::id() const { return id_; }
  inline const sql_arr& gsql_q::g_sql() const { return sql_; }
  inline cstr_t         gsql_q::sql(uint ndx) const
  {
    assert(ndx < sql_.size()); // NOLINT
    return sql_.at(ndx);
  }
}; // namespace dbgen3

#endif // GSQL_Q_HPP
