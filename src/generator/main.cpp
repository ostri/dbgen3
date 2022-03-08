#include <gflags/gflags.h>
#include <glog/logging.h>
#include <iostream>
#include <magic_enum.hpp>

#include "cmd_param/cmdline_parameters.hpp"
#include "core/executor.hpp"
#include "exceptions/exceptions.hpp"
#include "gsql_parser/core_parser.hpp"
#include "gsql_parser/xsd_grammar.hpp"
#include "utility_classes/common.hpp"
#include "utility_classes/program_status.hpp"
#include "utility_classes/string_format.hpp"
#include "version.hpp"

// clang-format off
DEFINE_string(db_name,        "",      "database name");                           // NOLINT
DEFINE_string(out_folder,     "./",    "folder for generated files");              // NOLINT
DEFINE_string(lang,           "cpp",   "language we generate source code in");     // NOLINT
DEFINE_string(db_type,        "db2",   "database type");                           // NOLINT
DEFINE_string(verbose,        "FALSE", "verbosity");                               // NOLINT
DEFINE_bool  (grammar,        false,   "display xsd grammar");                     // NOLINT
DEFINE_bool  (types,          false,   "display supported database column types"); // NOLINT
// clang-format on

using out = dbgen3::string_format;
using str_t = std::string;

int main(int argc, char** argv)
{
  auto sts = dbgen3::P_STS::success;
  str_t msg;
  try
  {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_log_dir = "/tmp";
    google::InitGoogleLogging(argv[0]); // NOLINT
    dbgen3::cmdline_parameters cmd_par(argc, argv);
    info << "version: " << get_version();
    info << cmd_par.dump("command line parameters:");
    sts = cmd_par.check_parameters();
    if (sts == dbgen3::P_STS::success)
    { /* parameters are OK */
      info << out::sl("parameters are ok.", 0);
      if (cmd_par.is_grammar()) dbgen3::executor::dsp_grammar();
      else if (cmd_par.is_types()) dbgen3::executor::dsp_types();
      else
      {
        dbgen3::executor e(cmd_par);
        return e.process_files();
      }
    }
    else { /* invalid or missing parameters */
      msg = out::sl(dbgen3::PS::dscr(sts), 0);
    }
  }
  catch (const dbgen3::gsql_file_not_exsts& e)
  {
    msg = out::sl(dbgen3::PS::dscr(e.status()));
    sts = e.status();
  }
  catch (const dbgen3::dbgen3_exc& e)
  {
    msg = out::sl(e.what());
    sts = e.status();
  }
  catch (const std::runtime_error& e)
  {
    msg = str_t("internal error:'") + e.what() + "'\n";
    sts = dbgen3::P_STS::unk_exception;
  }
  catch (...)
  {
    msg = "unknown exception\n";
    sts = dbgen3::P_STS::unknown_error;
  }
  err << msg;
  return dbgen3::ME::enum_integer<dbgen3::P_STS>(sts);
}
