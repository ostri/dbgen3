/*!
 * \file
 * \brief Declaration of class db::connection
 *
 */
#ifndef CONNECTION_H
#define CONNECTION_H

#include <atomic>
#include <sstream>
#include <vector>

#include <sqlcli1.h>

//#include "common.hpp"

/// dbgen2 runtime
namespace db
{
  /// prototype for log function callback
  typedef void (*err_log)(const std::string& msg);


  /*!
   * \brief It implements a connection to db via db2 CLI
   *
   * \ingroup runtime
   *
   * It handles database connection & transactions
   */
  class connection
  {
  public:
    //-----------------------------------------------------------------------
    //! @name Constructor & Destructor
    //{@
    /// constructor
    connection(const std::string& a_database,
               const std::string& a_user       = "",
               const std::string& a_password   = "",
               err_log            a_log        = 0,
               bool               show_log     = false,
               const int          a_cache_size = 5);
    /// destructor
    virtual ~connection();
    /// copy constructor
    connection(const connection& o);
    //@}
    void allocate_env_handle() const;
    //-----------------------------------------------------------------------
    //! @name Getters
    //{@
    /// get environment handle
    static SQLHANDLE get_env();
    /// get connection handle
    SQLHANDLE get_connection() const;
    /// get transaction count
    int get_tran_count() const;
    /// get log callback function
    err_log get_log() const;
    /// get statement handle
    SQLHANDLE get_free_handle();
    //@}
    //-----------------------------------------------------------------------
    /// @name Utility methods
    //{@
    /// begin transaction
    SQLRETURN begin_transaction();
    /// commit transactions
    SQLRETURN commit();
    /// rollback transactions
    SQLRETURN rollback();
    /// log an event
    ///void log(std::string msg) const;
    void log(const std::string& msg) const;
    /// store statement handle for further reuse
    SQLRETURN release_stmt_handle(SQLHANDLE h);
    //@}
    //-----------------------------------------------------------------------
    /// @name Setters
    //{@
    /// log setter
    void set_log(err_log a_log);
    /// transaction count setter
    void set_tran_count(int a_tran_count);
    /// should we show log or not (true - show, false - disable)
    void show_log(bool enable);
    /// set connection attribute
    SQLHANDLE set_attribute(const int an_attr, const int a_val) const;
    //    bool is_copy() const;
    const std::string& get_db_name() const;
    //@}
  private:
    /// display error context
    static void print_ctx(const std::string& msg,
                          const int          err_code,
                          const int          line,
                          const std::string& file);
    /// open new connection when environmen thandle is already there
    void open_connection(const std::string& a_database,
                         const std::string& a_user,
                         const std::string& a_password,
                         const uint         a_cache_size);

    /// environment handle
    static SQLHANDLE m_henv;
    /// number of connections referencing the same environment handle
    static std::atomic_uint m_henv_ref_cnt;
    /// lock mutex
    static std::mutex mtx;
    /// connection handle
    SQLHANDLE m_hdbc;
    /// log callback function
    err_log m_log;
    /// transaction depth indicator: begin_tran +1, commit | rollback -1
    int m_tran_count;
    /// true - display logs, false - do not display logs
    bool m_show_log;
    /// vector of statement handles that can be reused
    std::vector<SQLHANDLE> m_handle_cache;
    /// handle cache size
    uint m_handle_cache_size;
    /// database name
    std::string m_db_name;
    /// username
    std::string m_user;
    /// password
    std::string m_pass;
  };
  /// Serialisation to string stream
  std::ostream& operator<<(std::ostream& s, const connection& o);
} // namespace db

#endif
