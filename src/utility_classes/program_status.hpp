#ifndef PROGRAM_STATUS_H
#define PROGRAM_STATUS_H
#include <cassert>
#include <cstdint>
#include <string>
#include <sys/types.h>
#include <vector>

namespace dbgen3
{
  /**
	 * @brief program return status code
	 *
	 */
  enum class p_sts : uint
  {
    success              = 0,
    no_db_name           = 1,
    unknown_db_type      = 2,
    unknown_lang         = 3,
    out_folder_not_exist = 4,
    no_gsql_files        = 5,
  };

  /**
	 * @brief instance denotes one program status description
	 *
	 */
  struct status_dscr
  {
    p_sts       code;             //!< status code 0..255
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
			{p_sts::success,               "Program successfuly finished"	},
			{p_sts::no_db_name,            "Database name was not provided"},
			{p_sts::unknown_db_type,       "Datatbase type was not provided or unknown"},
			{p_sts::unknown_lang,          "Target programming language was not provided or unknown" },
			{p_sts::out_folder_not_exist,  "Output folder was not provided or does not exist"},
			{p_sts::no_gsql_files,         "No gsql filenames provided."},
    }){};
    // clang-format on
    ~program_status()                     = default;
    program_status(const program_status&) = default;
    program_status(program_status&&)      = default;
    program_status&    operator=(const program_status&) = delete;
    program_status&    operator=(program_status&&) = delete;
    const std::string& g_dscr(p_sts code);

  private:
    const std::vector<status_dscr> dic_; //!< dictionary of relevant statuses
  };
}; // namespace dbgen3
#endif
