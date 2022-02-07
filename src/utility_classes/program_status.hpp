#ifndef PROGRAM_STATUS_H
#define PROGRAM_STATUS_H
#include <cassert>
#include <cstdint>
#include <map>
#include <string>
#include <sys/types.h>
#include <vector>

#include "enums.hpp"

namespace dbgen3
{
  /**
   * @brief program return status code
   *
   */
  enum class P_STS
  {
    success                = 0,
    no_db_name             = 1,
    unknown_db_type        = 2,
    unknown_lang           = 3,
    out_folder_not_exist   = 4,
    no_gsql_files          = 5,
    unk_db_name            = 6,
    unk_exception          = 7,
    gsql_file_not_exists   = 8,
    duplicate_sql_buf_def  = 9,
    duplicate_sql_dscr_def = 10,
    sql_stat_too_long      = 11,
    cant_open_for_writing  = 12,
  };
  namespace
  {
    // clang-format off
    const std::map<P_STS, const char*> dic
    {
			{P_STS::success,                "Program successfuly finished"	}, //
			{P_STS::no_db_name,             "Database name was not provided"}, //
			{P_STS::unknown_db_type,        "Datatbase type was not provided or unknown. '{}'"}, //FIXME
			{P_STS::unknown_lang,           "Target programming language was not provided or unknown. '{}'" }, //FIXME
			{P_STS::out_folder_not_exist,   "Output folder was not provided or does not exist. '{}'"}, //FIXME
			{P_STS::no_gsql_files,          "No gsql filenames provided."},
      {P_STS::unk_db_name,            "Unknown db name provided. '{}'"}, //FIXME
      {P_STS::unk_exception,          "Unknown exception"},
      {P_STS::gsql_file_not_exists,   "Provided GSQL file '{}' does not exist or is not accesible."},
      {P_STS::duplicate_sql_buf_def,  "Duplicate SQL buffer definition. query id: {} sql#: {} rdbms: {} phase: {}"},
      {P_STS::duplicate_sql_dscr_def, "Duplicate SQL description definition. query id: {}"},
      {P_STS::sql_stat_too_long,      "SQL statement length exceeds hard limit {}. Shorten it. query id: {} sql#: {}"},
      {P_STS::cant_open_for_writing,  "Can't open file '{}' for  writing."},
    };
    // clang-format on

  } // namespace
  /**
   * @brief dictionary of program exit statuses
   *
   */
  class program_status
  {
  public:
    static cstr_t dscr(const P_STS& code) { return dic.at(code); }
  private:
  };
  using PS = program_status;
}; // namespace dbgen3
#endif
