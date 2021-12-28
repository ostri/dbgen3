#ifndef guard__CMDLINE_PARAMETERS_HPP_CRD5PNN6AA03__
#define guard__CMDLINE_PARAMETERS_HPP_CRD5PNN6AA03__

#include "common.hpp"
namespace dbgen3
{
  /// list of supported language generators
  enum class prog_lang
  {
    invalid = 0,
    cpp     = 1, //!< c++ code generation
    java    = 2  //!< java code generation (currently only a placeholder - no support)
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
    ~cmdline_parameters();
    cmdline_parameters(const cmdline_parameters& other) = default;
    cmdline_parameters(cmdline_parameters&& other)      = default;
    str_t dump(int offs = 0) const;
    /// @name getters
    ///@{
    auto g_db_name() const;       //!< fetch database name
    auto g_out_folder() const;    //!< fetch output folder
    auto g_qsql_list() const;     //!< fetch list of gsql filenames
    auto g_lang() const;          //!< fetch generator language
    auto g_database_type() const; //!< fetch database type
    ///@}
    prog_lang g_lang_code(str_t token)
    {
      std::cerr << "'" << token << "'-\n";
      auto res = (prog_lang)g_token_ndx(token, lang_str);
      std::cerr << "'" << (int)res << "'-\n";
      return res;
    }
    db_type       g_db_type_code(str_t token) { return (db_type)g_token_ndx(token, db_type_str); }
    const str_vec g_filenames_from_argv(int argc, char** argv) const;
    auto          g_lang_str(prog_lang token) const { return g_token_str((int)token, lang_str); }
    auto g_db_type_str(db_type token) const { return g_token_str((int)token, db_type_str); }

  private: /* const */
    /// programming languages names
    const str_vec lang_str;
    /// database type names
    const str_vec db_type_str;

  private:
    std::string db_name_;    //!< name of the database
    std::string out_folder_; //!< output folder
    str_vec     qsql_list_;  //!< vector of query file names
    prog_lang   lang_;       //!< programming language that we generate the code for
    db_type     database_type_ = db_type::db2; //!< database type that we are generate code for
  };
};     // namespace dbgen3
#endif // guard__CMDLINE_PARAMETERS_HPP_CRD5PNN6AA03__
