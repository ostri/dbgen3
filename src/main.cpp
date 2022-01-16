#include <gflags/gflags.h>
#include <glog/logging.h>

#include "cmdline_parameters.hpp"
#include "common.hpp"
#include "gsql_parser.hpp"
#include "program_status.hpp"
#include "string_format.hpp"

// clang-format off
DEFINE_string(db_name,        "", "database name");                          // NOLINT
DEFINE_string(out_folder,     "./", "folder for generated files");           // NOLINT
DEFINE_string(lang,           "cpp", "language we generate source code in"); // NOLINT
DEFINE_string(db_type,        "db2", "database type");                       // NOLINT
DEFINE_string(verbose,        "FALSE", "verbosity");                         // NOLINT
// clang-format on

using out = dbgen3::string_format;

int main(int argc, char** argv)
{
  try
  {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_log_dir = "/tmp";
    google::InitGoogleLogging(argv[0]); // NOLINT
    dbgen3::cmdline_parameters cmd_par(argc, argv);
    info << cmd_par.dump("command line parameters:");
    auto sts = cmd_par.check_parameters();
    if (sts == dbgen3::P_STS::success)
    { /* parameters are OK */
      info << out::sl("parameters are ok.", 0);
      xercesc::XMLPlatformUtils::Initialize();
      dbgen3::gsql_parser qe; // xercesc environment
      qe.parse_set(cmd_par.g_qsql_list());
      if (qe.isValid()) { info << out::sl("environment is ok"); }
    }
    else { /* invalid or missing parameters */
      auto tmp = dbgen3::program_status().dscr(sts);
      err << out::sl(tmp, 0);
    }
    return static_cast<int>(sts);
  }
  catch (const dbgen3::gsql_file_not_exsts& e)
  {
    err << out::sl(dbgen3::program_status().dscr(e.g_status()));
  }
  catch (const std::runtime_error& e)
  {
    err << "internal error " << out::sl(e.what());
  }
  catch (...)
  {
    std::cerr << "unknown exception" << '\n';
  }
  // TODO each exception should return distinct error code
}
