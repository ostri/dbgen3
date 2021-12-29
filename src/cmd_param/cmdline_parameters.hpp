#ifndef CMDLINE_PARAMETERS_HPP
#define CMDLINE_PARAMETERS_HPP

#include "common.hpp"
#include "program_status.hpp"

namespace dbgen3
{
  /// list of supported language generators
  enum class prog_lang
  {
    invalid = 0, //!< placeholder for all invalid inputs
    cpp     = 1, //!< c++ code generation
    java    = 2  //!< java code generation
  };

  /// list of supported RDBMS
  enum class db_type : std::int8_t
  {
    invalid = 0, //!< invalid database type
    db2     = 1, //!< IBM db2
    oracle  = 2, //!< Oracle database - placeholder
    mssql   = 3, //!< Microsoft SQL server - placeholder
    mariadb = 4  //!< Maria database (MYSQL clone) - placeholder
  };
  ///////////////////////////////////////////////////////////
  /// @brief cmdline_parameters
  ///
  class cmdline_parameters
  {
  public:
    cmdline_parameters(int    an_argc, //!< number of files to be processed
                       char** an_argv  //!< names of the files to be processed
    );
    ~cmdline_parameters()                               = default;
    cmdline_parameters(const cmdline_parameters& other) = default;
    cmdline_parameters(cmdline_parameters&& other)      = default;
    cmdline_parameters& operator=(const cmdline_parameters& o) = delete;
    cmdline_parameters& operator=(const cmdline_parameters&& o) = delete;
    /// check whether the parameters are ok
    p_sts check_parameters() const;
    /// return contents of the instance as a string (e.g. for logging)
    str_t dump(const str_t& msg = "", int offs = 0) const;
    /// @name getters
    ///@{
    auto g_db_name() const;       //!< fetch database name
    auto g_out_folder() const;    //!< fetch output folder
    auto g_qsql_list() const;     //!< fetch list of gsql filenames
    auto g_lang() const;          //!< fetch generator language
    auto g_database_type() const; //!< fetch database type
    auto g_verbose() const;       //!< fetch verbosity type
    ///@}
    /// @name auxiliary methods
    ///@{
    prog_lang      g_lang_code(const str_t& token) const;
    db_type        g_db_type_code(const str_t& token) const;
    static str_vec g_filenames_from_argv(int argc, char** argv);
    const str_t&   g_lang_str(prog_lang token) const;
    const str_t&   g_db_type_str(db_type token) const;
    const str_t&   g_verbose_str(bool token) const;

    ///@}

  private:
    const str_vec lang_str;    //!< programming languages names
    const str_vec db_type_str; //!< database type names
    const str_vec verbose_str; //!< verbosity names

    std::string db_name_;    //!< name of the database
    std::string out_folder_; //!< output folder
    str_vec     gsql_list_;  //!< vector of query file names
    prog_lang   lang_ //!< programming language that we generate the code for
      = prog_lang::cpp;
    db_type database_type_ //!< database type that we are generate code for
      = db_type::db2;
    bool verbose_ = false; //!< verbose program operation
  };
}; // namespace dbgen3

#endif // CMDLINE_PARAMETERS_HPP
