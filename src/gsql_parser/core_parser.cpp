#include "core_parser.hpp"
#include "xerces_strings.hpp"

namespace dbgen3
{
  namespace x = xercesc;
  core_parser::core_parser()
  : valid_(false)
  , init_(g_init())
  , impl_(xercesc::DOMImplementationRegistry::getDOMImplementation(u"LS"))
  , parser_(static_cast<xercesc::DOMImplementationLS*>(impl_)->createLSParser(
      xercesc::DOMImplementationLS::MODE_SYNCHRONOUS,
      nullptr))
  , eh_(new gsql_eh())
  {
    try
      {
        valid_ = true;
        parser_->getDomConfig()->setParameter(x::XMLUni::fgDOMValidate, // NOLINT
                                              true);
        parser_->getDomConfig()->setParameter(
          x::XMLUni::fgDOMNamespaces, // NOLINT
          true);
        parser_->getDomConfig()->setParameter(
          x::XMLUni::fgDOMDatatypeNormalization, true); // NOLINT
        /// error handler
        parser_->getDomConfig()->setParameter(
          xercesc::XMLUni::fgDOMErrorHandler, // NOLINT
          eh_.get());
      }
    catch (const x::XMLException& e)
      {
        auto msg = toNative(e.getMessage());
        err << "Error during initialization! :\n"
            << msg << "\n";
      }
  }

  core_parser::~core_parser()
  {
    if (parser_ != nullptr)
      {
        parser_->release();
        parser_ = nullptr;
      }
    if (init_)
      {
        x::XMLPlatformUtils::Terminate();
        init_ = false;
      }
  }

  bool core_parser::g_init()
  {
    if (! init_)
      {
        xercesc::XMLPlatformUtils::Initialize();
        init_ = true;
      }
    return init_;
  }
} // namespace dbgen3
