#ifndef CMDLINE_PARAMETERS_HPP
#define CMDLINE_PARAMETERS_HPP

//#include "magic_enum.hpp"

#include "common.hpp"
#include "enums.hpp"
#include "program_status.hpp"

namespace dbgen3
{

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
    P_STS check_parameters() const;
    /// return contents of the instance as a string (e.g. for logging)
    str_t dump(const str_t& msg = "", int offs = 0) const;
    /// @name getters
    ///@{
    cstr_t           db_name() const;    //!< fetch database name
    cstr_t           out_folder() const; //!< fetch output folder
    PROG_LANG        lang() const;
    static PROG_LANG lang_code(const std::string& token);
    RDBMS            database_type() const; //!< fetch database type
    static RDBMS     db_type_code(const std::string& token);
    VERBOSE          verbose_code() const { return verbose_; };
    static VERBOSE   verbose(const std::string& val);
    static str_vec   filenames_from_argv(int argc, char** argv);
    const str_vec&   qsql_list() const; //!< fetch list of gsql filenames
    // const str_t&   g_lang_str(const PROG_LANG& token) const;
    // const str_t&   g_db_type_str(db_type token) const;
    // const str_t&   g_verbose_str(bool token) const;

    ///@}
  private:
    //    const str_vec lang_str;    //!< programming languages names
    // const str_vec db_type_str; //!< database type names
    // const str_vec verbose_str; //!< verbosity names

    std::string db_name_;    //!< name of the database
    std::string out_folder_; //!< output folder
    str_vec     gsql_list_;  //!< vector of query file names
    PROG_LANG   lang_        //!< programming language that we generate the code for
      = PROG_LANG::cpp;
    RDBMS database_type_ //!< database type that we are generate code for
      = RDBMS::db2;
    VERBOSE verbose_ = VERBOSE::FALSE; //!< verbose program operation
  };

  inline VERBOSE cmdline_parameters::verbose(const std::string& val)
  {
    if (ME::enum_contains<VERBOSE>(val)) return ME::enum_cast<VERBOSE>(val).value();
    return VERBOSE::FALSE;
  }

  ///@}
  /// @name auxiliary methods
  ///@{
  inline PROG_LANG cmdline_parameters::lang_code(const std::string& token)
  {
    if (ME::enum_contains<PROG_LANG>(token)) return ME::enum_cast<PROG_LANG>(token).value();
    return PROG_LANG::invalid;
  }
}; // namespace dbgen3

#endif // CMDLINE_PARAMETERS_HPP
