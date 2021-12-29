#include <gflags/gflags.h>
#include <iostream>
#include <sstream>

#include "cmdline_parameters.hpp"
#include "string_format.hpp"

DECLARE_string(db_name);    // NOLINT
DECLARE_string(out_folder); // NOLINT
DECLARE_string(lang);       // NOLINT
DECLARE_string(db_type);    // NOLINT
namespace dbgen3
{
  /**
   * @brief Construct a new cmdline parameters::cmdline parameters object
   *
   * @param an_argc number of parameters (command line + qsql documents)
   * @param an_argv names of the files to be processed
   */
  cmdline_parameters::cmdline_parameters(int an_argc, char** an_argv)
  : lang_str({"invalid", "cpp", "java"})                          // NOLINT
  , db_type_str({"invalid", "db2", "oracle", "mssql", "mariadb"}) // NOLINT
  , db_name_(FLAGS_db_name)
  , out_folder_(FLAGS_out_folder)
  , qsql_list_(g_filenames_from_argv(an_argc, an_argv))
  , lang_(g_lang_code(FLAGS_lang))
  , database_type_(g_db_type_code(FLAGS_db_type))
  {
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
#pragma unroll 10
    for (const auto& file : this->qsql_list_)
    {
      files += out::sl(file, offs + 2);
    }
    return out::sl(msg, offs) + out::sl(" db name:    ", db_name_, offs) +
           out::sl(" out folder : ", out_folder_, offs) +
           out::s(" qsql files : ",
                  files.size() > 1 ? files : "no files" + out::ln(),
                  offs) +
           out::sl(" lang       : ", g_lang_str(lang_), offs) +
           out::sl(" db type    : ", g_db_type_str(database_type_), offs);
  }

  auto cmdline_parameters::g_db_name() const { return cstr_t(db_name_); }
  auto cmdline_parameters::g_out_folder() const { return cstr_t(out_folder_); }
  auto cmdline_parameters::g_qsql_list() const { return qsql_list_; }
  auto cmdline_parameters::g_lang() const { return lang_; }
  auto cmdline_parameters::g_database_type() const { return database_type_; }

  prog_lang cmdline_parameters::g_lang_code(const str_t& token) const
  {
    return static_cast<prog_lang>(g_token_ndx(token, lang_str));
  }

  db_type cmdline_parameters::g_db_type_code(const str_t& token) const
  {
    return static_cast<db_type>(g_token_ndx(token, db_type_str));
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

  const str_t& cmdline_parameters::g_lang_str(prog_lang token) const
  {
    return g_token_str(static_cast<int>(token), lang_str);
  }

  const str_t& cmdline_parameters::g_db_type_str(db_type token) const
  {
    return g_token_str(static_cast<int>(token), db_type_str);
  }
} // namespace dbgen3
