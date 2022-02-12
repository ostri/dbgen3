
#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <fstream>
#include <memory>

#include "cmdline_parameters.hpp"
#include "core_parser.hpp"
#include "db2_reader.hpp"
#include "gen.hpp"
#include "gen_cpp.hpp"
#include "gsql_q_set.hpp"

namespace dbgen3
{
  //namespace fs = std::filesystem;
  class executor
  {
  public:
    explicit executor(cmdline_parameters cmd_p);
    ~executor();
    executor(const executor&) = delete;
    executor(executor&&) = delete;
    executor& operator=(const executor&) = delete;
    executor& operator=(executor&&) = delete;
    int process_files();
    static x::DOMLSParser* create_parser(x::XMLGrammarPool* pool);
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
