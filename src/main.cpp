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
DEFINE_bool  (verbose, false, "verbosity");                                  // NOLINT
// clang-format on

using out = dbgen3::string_format;

int main(int argc, char** argv)
{
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  FLAGS_log_dir = "/tmp";
  google::InitGoogleLogging(argv[0]); // NOLINT
  dbgen3::cmdline_parameters cmd_par(argc, argv);
  info << cmd_par.dump("command line parameters:");
  auto sts = cmd_par.check_parameters();
  if (sts == dbgen3::p_sts::success)
  {
    info << out::sl("parameters are ok.", 0);
    xercesc::XMLPlatformUtils::Initialize();
    dbgen3::gsql_parser qe; // xercesc environment
    if (qe.isValid()) { info << out::sl("environment is ok"); }
  }
  else {
    auto tmp = dbgen3::program_status().g_dscr(sts);
    err << out::sl(tmp, 0);
  }
  return static_cast<int>(sts);
}
