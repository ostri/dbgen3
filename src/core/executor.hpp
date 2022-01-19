
#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "cmdline_parameters.hpp"
#include "gsql_q_set.hpp"
#include "core_parser.hpp"

namespace dbgen3
{
  class executor
  {
  public:
    executor(const cmdline_parameters& cmd_p)
    : cmd_p_(cmd_p)
    { }
    int process_files()
    {
      auto        sts = 0;
      core_parser p;
      for (const auto& file : cmd_p_.g_qsql_list())
      {
        auto dscr = p.parse_file(file);
        err << dscr.dump("finale");
      }
      return sts;
    }
  protected:
  private:
    const cmdline_parameters cmd_p_;  //!< command line parameters
    gsql_q_set               gsql_p_; //!< contents of the
  };
}; // namespace dbgen3

#endif // EXECUTOR_HPP
