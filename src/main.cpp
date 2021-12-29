#include <gflags/gflags.h>
#include <glog/logging.h>

#include "cmdline_parameters.hpp"

DEFINE_string(db_name, "", "database name");                       // NOLINT
DEFINE_string(out_folder, "/tmp", "folder for generated files");   // NOLINT
DEFINE_string(lang, "cpp", "language we generate source code in"); // NOLINT
DEFINE_string(db_type, "db2", "database type");                    // NOLINT

int main(int argc, char** argv)
{
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  FLAGS_log_dir = "/tmp";
  google::InitGoogleLogging(argv[0]); // NOLINT
  dbgen3::cmdline_parameters cmd_par(argc, argv);
  info << cmd_par.dump("command line parameters:");
  return 0;
}
