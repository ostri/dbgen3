
#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <fstream>
#include <memory>

#include "cmd_param/cmdline_parameters.hpp"
#include "db_integration/db2_reader.hpp"
#include "gen/gen.hpp"
#include "gen/gen_cpp.hpp"
#include "gsql_parser/core_parser.hpp"
#include "gsql_parser/gsql_q_set.hpp"
#include "gsql_parser/xsd_grammar.hpp"
#include "runtime/odbc_db.hpp"

namespace dbgen3
{
  // namespace fs = std::filesystem;
  class executor
  {
  public:
    explicit executor(cmdline_parameters cmd_p);
    ~executor();
    executor(const executor&)      = delete;
    executor(executor&&)           = delete;
    executor&              operator=(const executor&) = delete;
    executor&              operator=(executor&&) = delete;
    int                    process_files();
    static x::DOMLSParser* create_parser(x::XMLGrammarPool* pool);
    static void            dsp_grammar();
    static void            dsp_types();
  protected:
  private:
    static x::Grammar* load_grammar(x::XMLGrammarPool* gp);
    /*................................................................*/
    const cmdline_parameters cmd_p_;    //!< command line parameters
    gsql_q_set               gsql_p_;   //!< contents of the GSQL file
    std::ofstream            hpp_file_; //!< generated hpp file (output)
  };
}; // namespace dbgen3

#endif // EXECUTOR_HPP
