#ifndef PROGRAM_STATUS_H
#define PROGRAM_STATUS_H
#include <cassert>
#include <cstdint>
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
  };

  /**
   * @brief instance denotes one program status description
   *
   */
  struct status_dscr
  {
    P_STS       code;             //!< status code 0..255
    std::string dscr;             //!< human readable description of the status
  } __attribute__((aligned(64))); // NOLINT
  /**
   * @brief dictionary of program exit statuses
   *
   */
  class program_status
  {
  public:
    // clang-format off
    program_status()
    : dic_
		({
			{P_STS::success,               "Program successfuly finished"	}, //
			{P_STS::no_db_name,            "Database name was not provided"}, //
			{P_STS::unknown_db_type,       "Datatbase type was not provided or unknown. '{}'"}, //FIXME
			{P_STS::unknown_lang,          "Target programming language was not provided or unknown. '{}'" }, //FIXME
			{P_STS::out_folder_not_exist,  "Output folder was not provided or does not exist. '{}'"}, //FIXME
			{P_STS::no_gsql_files,         "No gsql filenames provided."},
      {P_STS::unk_db_name,           "Unknown db name provided. '{}'"}, //FIXME
      {P_STS::unk_exception,         "Unknown exception"},
      {P_STS::gsql_file_not_exists,  "Provided GSQL file does not exist. '{}'"}, //FIXME
      {P_STS::duplicate_sql_buf_def, "Duplicate SQL buffer definition. query id: {} sql#: {} rdbms: {} phase: {}"},
      {P_STS::duplicate_sql_dscr_def, "Duplicate SQL description definition. query id: {}"},
      {P_STS::sql_stat_too_long,     "SQL statement length exceeds hard limit {}. Shorten it. query id: {} sql#: {}"},
    }){};
    // clang-format on
    ~program_status()                     = default;
    program_status(const program_status&) = default;
    program_status(program_status&&)      = default;
    program_status&    operator=(const program_status&) = delete;
    program_status&    operator=(program_status&&) = delete;
    const std::string& dscr(const P_STS& code) const
    {
      uint ndx = ME::enum_integer<P_STS>(code);
      assert(ndx < dic_.size());
      return dic_.at(ndx).dscr;
    }
  private:
    const std::vector<status_dscr> dic_; //!< dictionary of relevant statuses
  };
}; // namespace dbgen3
#endif
