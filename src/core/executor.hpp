
#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <fstream>

#include "cmdline_parameters.hpp"
#include "gsql_q_set.hpp"
#include "core_parser.hpp"
#include "db2_reader.hpp"

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
    // static std::ofstream& open_out_file(const FILE_TYPE&          ft,
    //                                     const cmdline_parameters& cmd_p,
    //                                     cstr_t                    a_file)
    // {
    //   std::ofstream o_stream;
    //   fs::path      p(a_file);
    //   auto          fn = std::string(cmd_p.out_folder()) + fs::path::preferred_separator +
    //   p.stem().string() +
    //             ME::enum_name<FILE_TYPE>(ft);
    //   o_stream.open(fn, std::ios::out | std::ios::app);
    //   if (! o_stream.is_open()) throw dbgen3_exc(P_STS::cant_open_for_writing, fn);
    //   return o_stream;
    // }
    int process_files()
    {
      auto        sts = 0;
      core_parser p;
      db2_reader  r;
      r.connect(this->cmd_p_.g_db_name(), "", ""); // FIXME username and password also
      for (const auto& file : cmd_p_.g_qsql_list())
      {
        auto     gsql_struct = p.parse_file(file, r);
        fs::path p(file);
        fs::path fn(cmd_p_.out_folder());
        fn /= p.stem().string() + "." + std::string(ME::enum_name<FILE_TYPE>(FILE_TYPE::hpp));
        hpp_file_.open(fn);

        for (auto q : gsql_struct.q_dic()) { hpp_file_ << q.first << "\n"; }
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
