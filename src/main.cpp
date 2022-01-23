#include <gflags/gflags.h>
#include <glog/logging.h>
#include <magic_enum.hpp>

#include "cmdline_parameters.hpp"
#include "common.hpp"
#include "core_parser.hpp"
#include "program_status.hpp"
#include "string_format.hpp"
#include "exceptions.hpp"
#include "executor.hpp"

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
      xercesc::XMLPlatformUtils::Initialize(); /// FIXME why we need it here?
      dbgen3::executor e(cmd_par);
      e.process_files();
    }
    else { /* invalid or missing parameters */
      auto tmp = dbgen3::PS::dscr(sts);
      err << out::sl(tmp, 0);
    }
    return dbgen3::ME::enum_integer<dbgen3::P_STS>(sts);
  }
  catch (const dbgen3::gsql_file_not_exsts& e)
  {
    err << out::sl(dbgen3::PS::dscr(e.g_status()));
    return dbgen3::ME::enum_integer<dbgen3::P_STS>(e.g_status());
  }
  catch (const std::runtime_error& e)
  {
    err << "internal error " << out::sl(e.what());
    return dbgen3::ME::enum_integer<dbgen3::P_STS>(dbgen3::P_STS::unk_exception);
  }
  catch (...)
  {
    std::cerr << "unknown exception" << '\n';
  }
  // TODO each exception should return distinct error code
}
