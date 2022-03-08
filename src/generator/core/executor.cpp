#include <memory>
#include <stdexcept>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMLSParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>
#include <xercesc/framework/XMLGrammarPool.hpp>
#include <xercesc/framework/XMLGrammarPoolImpl.hpp>
#include <xercesc/validators/common/Grammar.hpp>
#include <filesystem>

#include "fmt/core.h"
#include "magic_enum.hpp"
#include "utility_classes/multi_line.hpp"
#include "utility_classes/program_status.hpp"
#include "gsql_parser/xsd_grammar.hpp"

#include "gsql_parser/dom_error_handler.hpp"
#include "exceptions/exceptions.hpp"
#include "core/executor.hpp"

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

  void executor::dsp_grammar()
  {
    std::cout << multi_line::to_str(     //
                   multi_line::minimize( //
                     multi_line::to_vec( //
                       grammar_)),
                   '\n')
              << std::endl;
  }

  void executor::dsp_types()
  {
    for (auto ndx = 1UL; ndx < db::size(); ++ndx)
    {
      const auto [db_type, c_type] = db::db_types(ndx);
      std::cerr << fmt::format("{:18} {} \n", db_type, c_type);
    }
  }
  x::Grammar* executor::load_grammar(x::XMLGrammarPool* gp)
  {
    x::Grammar*       gram = nullptr;
    auto *  parser(create_parser(gp));
    dom_error_handler eh;
    parser->getDomConfig()->setParameter(x::XMLUni::fgDOMErrorHandler, &eh); // NOLINT

    str_t g(trim(grammar_)); // to remove leading spaces; there must be nothing before
                             // first tag in the xml file
    // NOLINTNEXTLINE
    x::MemBufInputSource         mis(reinterpret_cast<const XMLByte*>(g.data()), g.size(), "xsd");
    xercesc::Wrapper4InputSource wmis(&mis, false);
    gram = parser->loadGrammar(&wmis, xercesc::Grammar::SchemaGrammarType, true);
    if (eh.failed())
    {
      auto fmt = PS::dscr(P_STS::inv_grammar_syntax);
      auto msg = fmt::format(fg(fmt::color::crimson), fmt.data(), eh.line(), eh.col(), eh.e_msg());
      //std::cerr << msg << std::endl;
      throw dbgen3_exc(P_STS::inv_grammar_syntax, msg);
    }
    parser->release();  //FIXME it leaks upon grammar error 
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
    x::MemoryManager*                  mm(x::XMLPlatformUtils::fgMemoryManager);
    std::unique_ptr<x::XMLGrammarPool> gp(new x::XMLGrammarPoolImpl(mm));
    auto*                              sts = load_grammar(gp.get());
    if (sts == nullptr) { return ME::enum_integer<P_STS>(P_STS::inv_grammar_syntax); }
    core_parser p(gp.get());
    db2_reader  r;
    r.connect(cmd_p_.db_name(), "", ""); // FIXME username and password also
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
      hpp_file_.close();
    }
    return ME::enum_integer<P_STS>(P_STS::success);
  }
}; // namespace dbgen3