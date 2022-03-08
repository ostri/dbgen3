#ifndef CMDLINE_PARAMETERS_HPP
#define CMDLINE_PARAMETERS_HPP

//#include "magic_enum.hpp"

#include "utility_classes/common.hpp"
#include "utility_classes/enums.hpp"
#include "utility_classes/program_status.hpp"

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

    bool is_grammar() const { return this->grammar_; }
    bool is_types() const { return this->types_; }

    ///@}
  private:
    std::string db_name_;                        //!< name of the database
    std::string out_folder_;                     //!< output folder
    str_vec     gsql_list_;                      //!< vector of query file names
    PROG_LANG   lang_          = PROG_LANG::cpp; //!< code is generated in this prog language
    RDBMS       database_type_ = RDBMS::db2;     //!< code it generated for this rdbms type
    VERBOSE     verbose_       = VERBOSE::FALSE; //!< verbose program operation
    bool        grammar_{};                      //!< should we display grammar
    bool        types_{};                        //!< should we display supported db types
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
