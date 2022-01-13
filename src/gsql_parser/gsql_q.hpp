#ifndef GSQL_ITEM_H
#define GSQL_ITEM_H

#include <array>
#include <utility>

#include "common.hpp"


namespace dbgen3
{
  /**
	 * @brief one SQL statement description within the gsql file
	 *
	 */
  class gsql_q
  {
    /**
		 * placeholder for sql statements
		 * 1 - main sql statement
		 * 0,2 needed for proper dbgen3 execution (e.g. work with the temporary tables)
	   */
    using sql_arr = std::array<str_t, 3>;

  public:
    gsql_q(const str_t& core_name, sql_arr sql)
    : core_name_(core_name)
    , p_buf_name_(core_name + "_par")
    , r_buf_name_(core_name + "_res")
    , sql_(std::move(sql))
    {
    }
    ~gsql_q()                = default;
    gsql_q(const gsql_q&) = default;
    gsql_q(gsql_q&&)      = default;
    gsql_q& operator=(const gsql_q&) = delete;
    gsql_q& operator=(gsql_q&&) = delete;
    /// @name getters
    ///@{
    cstr_t         g_core_name() const { return core_name_; }
    cstr_t         g_p_buf_name() const { return p_buf_name_; }
    cstr_t         g_r_buf_name() const { return r_buf_name_; }
    const sql_arr& g_sql() const { return sql_; }
    cstr_t         g_sql(uint ndx) const
    {
      assert(ndx < sql_.size()); // NOLINT
      return sql_.at(ndx);
    }
    ///@}
    /// @name setters
    ///@{
    void s_core_name(const str_t& o) { core_name_ = o; }
    void s_p_buf_name(const str_t& o) { p_buf_name_ = o; }
    void s_r_buf_name(const str_t& o) { r_buf_name_ = o; }
    void s_sql(const sql_arr& sql) { sql_ = sql; }
    void s_sql(uint ndx, const str_t& sql)
    {
      assert(sql_.size() == 3); // NOLINT
      assert(ndx < 3);          // NOLINT
      sql_[ndx] = sql;
    }
    ///@}

  private:
    str_t   core_name_;          //!< name of the gsql_id (unique id)
    str_t   p_buf_name_;         //!< parameter buffer name (if exists)
    str_t   r_buf_name_;         //!< result buffer name (if exists)
    sql_arr sql_ = {"", "", ""}; //!< set of sql statements in the gsql item
  };
}; // namespace dbgen3
#endif
