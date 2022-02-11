#include "executor.hpp"
namespace dbgen3
{
  namespace fs = std::filesystem;
  
  dbgen3::executor::executor(cmdline_parameters cmd_p)
  : cmd_p_(std::move(cmd_p))
  { }
  dbgen3::executor::~executor()
  {
    if (hpp_file_.is_open()) hpp_file_.close();
  }
  int dbgen3::executor::process_files()
  {
    auto        sts = 0;
    core_parser p;
    db2_reader  r;
    r.connect(this->cmd_p_.db_name(), "", ""); // FIXME username and password also
    std::unique_ptr<gen> gen;
    if (cmd_p_.lang() == PROG_LANG::cpp) gen = std::make_unique<gen_cpp>();
    assert(gen.get() != nullptr); // NOLINT clang-tidy(hicpp-no-array-decay)
    for (const auto& file : cmd_p_.qsql_list())
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
}; // namespace dbgen3