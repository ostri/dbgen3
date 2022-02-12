/*!
 * \file
 * \brief Declaration of class db::connection
 *
 */
#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <atomic>
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
namespace db
{

  using err_log = void (*)(const std::string&); //!< prototype for log function callback
  using str_t   = std::string;
  using cstr_t  = std::string_view;

  struct h_pair
  {
    std::int32_t handle;         //!< handle
    std::int32_t h_type;         //!< handle type
  } __attribute__((aligned(8))); // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
  using h_stack = std::stack<h_pair>;
  void _log_(const std::string& msg); //!< default log (std::cerr)
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
    /**
     * @brief name of the environment variable that triggers the logging
     *
     * Should one want to activate the logs in the runtime component, set the environment
     * variable with the provided name to non null value.
     */
    static constexpr const char* log_env_name = "DBGEN3_TRACE";
    //-----------------------------------------------------------------------
    //! @name Constructor & Destructor
    //{@
    /**
     * @brief Construct a new connection object
     *
     * @param a_database name of the database to connect
     * @param a_user     username for the database (or "" -> default assumed)
     * @param a_password corresonding password (or "" -> username of default user assumed)
     * @param a_log      logging calback method (see errlog; if none provided it logs to the
     * std::cerr)
     */
    explicit connection(cstr_t  a_database,
                        cstr_t  a_user     = "",
                        cstr_t  a_password = "",
                        err_log a_log      = nullptr);
    virtual ~connection();                                //!< destructor
    connection(const connection& o) = default;            //!< copy constructor
    connection(connection&& o)      = default;            //!< move constructor
    connection& operator=(const connection& o) = default; //!< assigment operator
    connection& operator=(connection&& o) = default;      //!< move assigment operator
    //@}
    //-----------------------------------------------------------------------
    //! @name Getters
    //{@
    static std::int32_t get_env();                         //!< get environment handle
    std::int32_t        get_db_handle() const;             //!< get connection handle
    err_log             get_log() const;                   //!< get log callback function
    std::int32_t        alloc_stmt_handle() const;         //!< get statement handle
    const std::string&  get_db_name() const;               //!< fetch database name
    static bool         should_we_log();                   //!< is runtime logging switched on?
    std::string dump(const std::string& a_msg = "") const; //!< serialize connection attributes
    //@}
    /// @name Setters
    //{@
    void         set_log(err_log a_log);                      //!< log handler setter
    std::int16_t set_attribute(int an_attr, int a_val) const; //!< set connection attribute
    //@}
    //-----------------------------------------------------------------------
    /// @name Utility methods
    //{@
    std::int16_t        commit() const;                      //!< commit transactions
    std::int16_t        rollback() const;                    //!< rollback transactions
    void                log(const std::string& msg) const;   //!< log an event
    static std::int16_t dealloc_stmt_handle(std::int32_t h); //!< release a statement handle
    //@}
  private:
    /**
     * @brief checks status code and throw on error
     *
     * @param rc status code (0 == ok)
     * @param a_handle db handle where we are checking status
     * @param a_handle_type db handle type where we are checking status
     * @param a_msg a messsage to be dispatched upon error
     */
    static h_pair throw_on_error(std::int16_t       rc,
                                 std::int32_t       a_handle,
                                 std::int16_t       a_handle_type,
                                 const std::string& a_msg);

    static h_pair allocate_env_handle(); //!< allocate database environment handle
    std::int32_t  alloc_stmt_handle(std::int32_t db_handle) const; //!< get statement handle

    static void print_ctx( //!< display error context
      const std::string& msg,
      int                err_code,
      int                line,
      const std::string& file);
    int         open_connection( //!< open new db connection
      const std::string& a_database,
      const std::string& a_user,
      const std::string& a_password);
    void        close_connection(); //!< close connection (invers of open connection)
    /*... instance attributes ................................................*/
    static std::int32_t henv_; //!< environment handle NOLINT
    // number of connections referencing the same environment handle
    static std::atomic_uint henv_ref_cnt_; // NOLINT
    //    h_stack                 h_stack_;      //!< handle stack to be exception safe
    err_log      log_;       //!< log callback function
    std::string  db_name_;   //!< database name
    std::string  user_;      //!< username
    std::string  pass_;      //!< password
    std::int32_t db_handle_; //<! connection handle
  };
  /// should we log the execution ?
  inline bool connection::should_we_log()
  {
    return std::getenv(log_env_name) != nullptr; // NOLINT clang-tidy(concurrency-mt-unsafe)
  }
} // namespace db

#endif // CONNECTION_HPP
