#include <gflags/gflags.h>
#include <glog/logging.h>
#include <iostream>

#include "cmdline_parameters.hpp"

DEFINE_string(db_name, "", "database name");
DEFINE_string(out_folder, "/tmp", "folder for generated files");
DEFINE_string(lang, "cpp", "language in which we generate source code");
DEFINE_string(db_type, "db2", "database type we are trying to access");

int main(int argc, char** argv)
{
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  FLAGS_log_dir = "/tmp";
  google::InitGoogleLogging(argv[0]);
  std::cerr << "cmd_parameters" << argc << std::endl;
  dbgen3::cmdline_parameters cmd_par(argc, argv);
  std::cerr << "cmd_parameters 2" << argc << std::endl;
  LOG(INFO) << "starting program \n" << cmd_par.dump(0);
  return 0;
}
