
#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "cmdline_parameters.hpp"
#include "gsql_q_set.hpp"
#include "core_parser.hpp"
#include "db2_reader.hpp"

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
      db2_reader  r;
      r.connect("test","","");
      for (const auto& file : cmd_p_.g_qsql_list())
      {
        auto gsql_struct = p.parse_file(file, r);
        for (auto q: gsql_struct.q_dic())
        {
          err << q.first << "\n";
        }
        err << gsql_struct.dump("finale");
      }
      return sts;
    }
  protected:
  private:
    const cmdline_parameters cmd_p_;  //!< command line parameters
    gsql_q_set               gsql_p_; //!< contents of the GSQL file
  };
}; // namespace dbgen3

#endif // EXECUTOR_HPP
