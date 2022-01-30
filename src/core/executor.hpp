
#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <fstream>
#include <memory>

#include "cmdline_parameters.hpp"
#include "gsql_q_set.hpp"
#include "core_parser.hpp"
#include "db2_reader.hpp"
#include "gen.hpp"
#include "gen_cpp.hpp"

namespace dbgen3
{
  namespace fs = std::filesystem;
  class executor
  {
  public:
    executor(const cmdline_parameters& cmd_p)
    : cmd_p_(cmd_p)
    , hpp_file_()
    { }
    ~executor()
    {
      if (hpp_file_.is_open()) hpp_file_.close();
    }
    int process_files()
    {
      auto        sts = 0;
      core_parser p;
      db2_reader  r;
      r.connect(this->cmd_p_.g_db_name(), "", ""); // FIXME username and password also
      std::unique_ptr<gen> gen;
      if (cmd_p_.lang() == PROG_LANG::cpp) gen = std::make_unique<gen_cpp>();
      assert(gen.get() != nullptr);
      for (const auto& file : cmd_p_.g_qsql_list())
      {
        auto     gsql_struct = p.parse_file(file, r, cmd_p_.database_type());
        fs::path p(file);
        fs::path fn(cmd_p_.out_folder());
        fn /= p.stem().string() + "." + std::string(ME::enum_name<FILE_TYPE>(FILE_TYPE::hpp));
        hpp_file_.open(fn);
        gen->set_set(gsql_struct);
        gen->set_rdbm(cmd_p_.database_type());
        hpp_file_ << gen->gen_file(0);
        //        err << gsql_struct.dump("finale");
        hpp_file_.close();
      }
      return sts;
    }
  protected:
  private:
    const cmdline_parameters cmd_p_;    //!< command line parameters
    gsql_q_set               gsql_p_;   //!< contents of the GSQL file
    std::ofstream            hpp_file_; //!< generated hpp file (output)
  };
}; // namespace dbgen3

#endif // EXECUTOR_HPP
