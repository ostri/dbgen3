/*!
 * \file
 * \brief Declaration of class db::connection
 *
 */
#ifndef CONNECTION_H
#define CONNECTION_H

#include <sqlcli1.h>


#include <atomic>
#include <vector>


namespace db
{
  /// prototype for log function callback
  using err_log = void (*)(const std::string&);

  void _log_(const std::string& msg);

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
    explicit connection(const std::string& a_database,
                        const std::string& a_user     = "",
                        const std::string& a_password = "",
                        err_log            a_log      = nullptr,
                        bool               show_log   = false);
    virtual ~connection();                     //!< destructor
    connection(const connection& o) = default; //!< copy constructor
    connection(connection&& o)      = default; //!< move constructor
    connection& operator=(const connection& o) = default;
    connection& operator=(connection&& o) = default;
    //@}
    void allocate_env_handle() const;
    //-----------------------------------------------------------------------
    //! @name Getters
    //{@
    /// get environment handle
    static SQLHANDLE get_env();
    /// get connection handle
    SQLHANDLE get_connection() const;
    /// get log callback function
    err_log get_log() const;
    /// get statement handle
    SQLHANDLE get_free_handle() const;
    //@}
    //-----------------------------------------------------------------------
    /// @name Utility methods
    //{@
    /// commit transactions
    SQLRETURN commit() const;
    /// rollback transactions
    SQLRETURN rollback() const;
    /// log an event
    ///void log(std::string msg) const;
    void log(const std::string& msg) const;
    /// store statement handle for further reuse
    SQLRETURN release_stmt_handle(SQLHANDLE h) const;
    //@}
    //-----------------------------------------------------------------------
    /// @name Setters
    //{@
    /// log setter
    void set_log(err_log a_log);
    /// should we show log or not (true - show, false - disable)
    void show_log(bool enable);
    /// set connection attribute
    SQLHANDLE set_attribute(int an_attr, int a_val) const;
    //    bool is_copy() const;
    const std::string& get_db_name() const;
    //@}
  private:
    /// display error context
    static void print_ctx(const std::string& msg,
                          int                err_code,
                          int                line,
                          const std::string& file);
    /// open new db connection
    int open_connection(const std::string& a_database,
                        const std::string& a_user,
                        const std::string& a_password);
    /*... instance attributes ................................................*/
    static SQLHANDLE m_henv; //!< environment handle NOLINT
    // number of connections referencing the same environment handle
    static std::atomic_uint m_henv_ref_cnt; // NOLINT

    err_log     m_log;       //!< log callback function
    bool        m_show_log;  //!< true - display logs, false - do not display logs
    std::string m_db_name;   //!< database name
    std::string m_user;      //!< username
    std::string m_pass;      //!< password
    SQLHANDLE   m_db_handle; //<! connection handle
  };
  /// Serialisation to string stream
  std::ostream& operator<<(std::ostream& s, const connection& o);
} // namespace db

#endif
