#include <gflags/gflags.h>
#include <iostream>
#include <sstream>

#include "cmdline_parameters.hpp"
#include "string_format.hpp"

DECLARE_string(db_name);    // NOLINT
DECLARE_string(out_folder); // NOLINT
DECLARE_string(lang);       // NOLINT
DECLARE_string(db_type);    // NOLINT
DECLARE_string(verbose);    // NOLINT
namespace dbgen3
{
  /**
   * @brief Construct a new cmdline parameters::cmdline parameters object
   *
   * @param an_argc number of parameters (command line + qsql documents)
   * @param an_argv names of the files to be processed
   */
  cmdline_parameters::cmdline_parameters(int an_argc, char** an_argv)
  : db_name_(FLAGS_db_name)
  , out_folder_(FLAGS_out_folder)
  , gsql_list_(g_filenames_from_argv(an_argc, an_argv))
  , lang_(g_lang_code(FLAGS_lang))
  , database_type_(g_db_type_code(FLAGS_db_type))
  , verbose_(g_verbose(std::string(FLAGS_verbose)))
  { }
  P_STS cmdline_parameters::check_parameters() const
  {
    if (db_name_.empty()) { return P_STS::no_db_name; };
    if (database_type_ == RDBMS::sql) { return P_STS::unknown_db_type; };
    if (lang_ == PROG_LANG::invalid) { return P_STS::unknown_lang; };
    if (! file_exists(out_folder_)) { return P_STS::out_folder_not_exist; };
    if (gsql_list_.empty()) { return P_STS::no_gsql_files; }
    return P_STS::success;
  }
  /**
   * @brief serialize the instnace content to the string it returns
   *
   * @param offs offset of the text form left margin
   * @return string contents of the instance
   */
  str_t cmdline_parameters::dump(const str_t& msg, int offs) const // NOLINT
  {
    std::string files("\n");
    for (const auto& file : this->gsql_list_) { files += out::sl(file, offs + 2); }
    // clang-format off
    return
    out::sl(msg, offs) +
    out::sl(" db name    : ", db_name_, offs) +
    out::sl(" out folder : ", out_folder_, offs) +
    out::s (" qsql files : ", files.size() > 1 ? files : "no files" + out::ln(),offs) +
    out::sl(" lang       : ", ME::enum_name(lang_), offs) +
    out::sl(" db type    : ", ME::enum_name(database_type_), offs)+
    out::sl(" verbose    : ", ME::enum_name(verbose_), offs)
    ;
    // clang-format on
  }

  cstr_t         cmdline_parameters::g_db_name() const { return cstr_t(db_name_); }
  cstr_t         cmdline_parameters::out_folder() const { return cstr_t(out_folder_); }
  const str_vec& cmdline_parameters::g_qsql_list() const { return gsql_list_; }

  RDBMS cmdline_parameters::g_database_type() const { return database_type_; }

  // TODO remove g_token_ndx
  RDBMS cmdline_parameters::g_db_type_code(const std::string& token) const
  {
    if (ME::enum_contains<RDBMS>(token)) return ME::enum_cast<RDBMS>(token).value();
    else return RDBMS::sql;
  }

  /**
   * @brief return list of the gsql filenames
   *
   * The method extracts the list of the qsql filenames from the command line
   * parameter list. First filename is name of the program (should be omitted).
   *
   * @param argc number of parameters in command line
   * @param argv list of parameters in the command line (first is the program name)
   * @return list of gsql files to be processed
   */
  str_vec cmdline_parameters::g_filenames_from_argv(int argc, char** argv)
  {
    if (argc > 1) return str_vec(argv + 1, argv + argc); // NOLINT
    return {};
  }

  // const str_t& cmdline_parameters::g_lang_str(const PROG_LANG& token) const
  // {
  //   return g_token_str(static_cast<int>(token), enums::lang_str());
  // }

  // const str_t& cmdline_parameters::g_db_type_str(db_type token) const
  // {
  //   return g_token_str(static_cast<int>(token), db_type_str);
  // }

  // const str_t& cmdline_parameters::g_verbose_str(bool token) const
  // {
  //   return token ? verbose_str.at(1) : verbose_str.at(0);
  // }
} // namespace dbgen3
