#include "cmdline_parameters.hpp"
#include "string_format.hpp"
#include <gflags/gflags.h>
#include <iostream>
#include <sstream>

DECLARE_string(db_name);
DECLARE_string(out_folder);
DECLARE_string(lang);
DECLARE_string(db_type);
namespace dbgen3
{
  /**
   * @brief Construct a new cmdline parameters::cmdline parameters object
   *
   * @param an_argc number of parameters (command line + qsql documents)
   * @param an_argv names of the files to be processed
   */
  cmdline_parameters::cmdline_parameters(int an_argc, char** an_argv)
  : lang_str({"invalid", "cpp", "java"})
  , db_type_str({"invalid", "db2", "oracle", "mssql", "mariadb"})
  , db_name_(FLAGS_db_name)
  , out_folder_(FLAGS_out_folder)
  , qsql_list_(g_filenames_from_argv(an_argc, an_argv))
  , lang_(g_lang_code(FLAGS_lang))
  , database_type_(g_db_type_code(FLAGS_db_type))
  {
    std::cerr << "constructor \n";
  }
  /**
   * @brief Destroy the cmdline parameters::cmdline parameters object
   *
   */
  cmdline_parameters::~cmdline_parameters() {}
  /**
   * @brief serialize the instnace content to the string it returns
   *
   * @param offs offset of the text form left margin
   * @return string contents of the instance
   */
  str_t cmdline_parameters::dump(int offs) const
  {
    return out::sl(" db name:    ", db_name_, offs) + out::sl(" out folder: ", out_folder_, offs) +
           out::sl(" lang:       ", g_lang_str(lang_), offs) +
           out::sl(" db type:    ", g_db_type_str(database_type_), offs);
  }

  auto cmdline_parameters::g_db_name() const { return cstr_t(db_name_); }
  auto cmdline_parameters::g_out_folder() const { return cstr_t(out_folder_); }
  auto cmdline_parameters::g_qsql_list() const { return qsql_list_; }
  auto cmdline_parameters::g_lang() const { return lang_; }
  auto cmdline_parameters::g_database_type() const { return database_type_; }

  /**
   * @brief return list of the gsql filenames
   *
   * The method extracts the list of the qsql filenames from the command line
   * parameter list. First filename is name of the program (should be omitted).
   *
   * @param argc number of parameters in command line
   * @param argv list of parameters in the command line (first is the program name)
   * @return
   */
  const str_vec cmdline_parameters::g_filenames_from_argv(int argc, char** argv) const
  {
    if (argc > 1) return str_vec(argv + 1, argv + argc);
    return str_vec();
  }
} // namespace dbgen3
