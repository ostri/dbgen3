#include "executor.hpp"
#include <stdexcept>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMLSParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>
#include <xercesc/framework/XMLGrammarPool.hpp>
#include <xercesc/framework/XMLGrammarPoolImpl.hpp>
#include <xercesc/validators/common/Grammar.hpp>

#include "dom_error_handler.hpp"
#include "exceptions.hpp"
#include "fmt/core.h"
#include "multi_line.hpp"
#include "program_status.hpp"
#include "xsd_grammar.hpp"
namespace dbgen3
{
  namespace fs = std::filesystem;
  executor::executor(cmdline_parameters cmd_p)
  : cmd_p_(std::move(cmd_p))
  {
    xercesc::XMLPlatformUtils::Initialize();
  }
  executor::~executor()
  {
    if (hpp_file_.is_open()) hpp_file_.close();
    xercesc::XMLPlatformUtils::Terminate();
  }
  x::DOMLSParser* executor::create_parser(x::XMLGrammarPool* pool)
  {
    x::DOMImplementation* impl(x::DOMImplementationRegistry::getDOMImplementation(u"LS"));
    x::DOMLSParser*       parser(impl->createLSParser(x::DOMImplementationLS::MODE_SYNCHRONOUS,
                                                nullptr,
                                                x::XMLPlatformUtils::fgMemoryManager,
                                                pool));
    x::DOMConfiguration*  c(parser->getDomConfig());
    // Commonly useful configuration.
    c->setParameter(x::XMLUni::fgDOMComments, false);                 // NOLINT
    c->setParameter(x::XMLUni::fgDOMDatatypeNormalization, true);     // NOLINT
    c->setParameter(x::XMLUni::fgDOMEntities, false);                 // NOLINT
    c->setParameter(x::XMLUni::fgDOMNamespaces, true);                // NOLINT
    c->setParameter(x::XMLUni::fgDOMElementContentWhitespace, false); // NOLINT
    // Enable validation.
    c->setParameter(x::XMLUni::fgDOMValidate, true);               // NOLINT
    c->setParameter(x::XMLUni::fgXercesSchema, true);              // NOLINT
    c->setParameter(x::XMLUni::fgXercesSchemaFullChecking, false); // NOLINT
    // Use the loaded grammar during parsing.
    c->setParameter(x::XMLUni::fgXercesUseCachedGrammarInParse, true); // NOLINT
    // Don't load schemas from any other source (e.g., from XML document's
    // xsi:schemaLocation attributes).
    c->setParameter(x::XMLUni::fgXercesLoadSchema, false); // NOLINT
    // Xerces-C++ 3.1.0 is the first version with working multi
    // import support.
#if _XERCES_VERSION >= 30100
    c->setParameter(x::XMLUni::fgXercesHandleMultipleImports, true); // NOLINT
#endif
    return parser;
  }
  x::Grammar* executor::load_grammar(x::XMLGrammarPool* gp)
  {
    //auto              sts = 0;
    x::Grammar*       gram = nullptr;
    x::DOMLSParser*   parser(create_parser(gp));
    dom_error_handler eh;
    parser->getDomConfig()->setParameter(x::XMLUni::fgDOMErrorHandler, &eh); // NOLINT

    //multi_line ml(grammar_);
    str_t      g(trim(grammar_)); // to remove leading spaces; there must be nothing before
                                  // first tag in the xml file
    // NOLINTNEXTLINE
    x::MemBufInputSource         mis(reinterpret_cast<const XMLByte*>(g.data()), g.size(), "xsd");
    xercesc::Wrapper4InputSource wmis(&mis, false);
    try
    {
      gram = parser->loadGrammar(&wmis, xercesc::Grammar::SchemaGrammarType, true);
    }
    // catch (const x::DOMException& e)
    // catch (const x::XMLException& e)
    catch (...)
    {
      if (eh.failed() /*|| gram == nullptr*/)
      {
        auto msg = fmt::format(fg(fmt::color::crimson),
                               PS::dscr(P_STS::inv_gsql_syntax),
                               eh.line(),
                               eh.col(),
                               eh.e_msg());
        err << msg << std::endl;
        throw dbgen3_exc(P_STS::inv_grammar_syntax, msg);
      }
    };
    parser->release();
    gp->lockPool();
    return gram;
  }
  /**
   * @brief process all input files
   *
   * @return int 0 - ok
   */
  int executor::process_files()
  {
    //auto                               sts = 0;
    x::MemoryManager*                  mm(x::XMLPlatformUtils::fgMemoryManager);
    std::unique_ptr<x::XMLGrammarPool> gp(new x::XMLGrammarPoolImpl(mm));
    auto *sts = load_grammar(gp.get());
    if (sts == nullptr) throw std::runtime_error("errors in grammar");
    core_parser p(gp.get());
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
    return 0;
  }
}; // namespace dbgen3