#include <xercesc/dom/DOMText.hpp>

#include "core_parser.hpp"
#include "xerces_strings.hpp"
#include "gsql_q_set.hpp"
#include "gsql_q.hpp"

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
      // parser_->getDomConfig()->setParameter(x::XMLUni::fgDOMValidate, // NOLINT
      //                                       true);
      parser_->getDomConfig()->setParameter(x::XMLUni::fgDOMNamespaces, // NOLINT
                                            true);
      parser_->getDomConfig()->setParameter(x::XMLUni::fgDOMDatatypeNormalization, true); // NOLINT
      /// error handler
      parser_->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMErrorHandler, // NOLINT
                                            eh_.get());
    }
    catch (const x::XMLException& e)
    {
      auto msg = toNative(e.getMessage());
      err << "Error during initialization! :\n" << msg << "\n";
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

  ///@}
  gsql_q_set core_parser::parse_file(const str_t& a_file)
  {
    try
    {
      auto doc = parser_->parseURI(a_file.data());
      if (doc != nullptr)
      {
        x::DOMNode* root = doc->getDocumentElement();
        auto y = load_q_set(root);
      };
    }
    catch (const x::XMLException& e)
    {
      err << out::sl("Exception message is: ", toNative(e.getMessage()), 0);
    }
    catch (const x::DOMException& e)
    {
      err << out::sl("Exception message is: ", toNative(e.getMessage()), 0);
    }
    catch (...)
    {
      err << out::sl("unknown exception", 0);
    };
    return {};
  }
  gsql_q_set core_parser::load_q_set(const x::DOMNode* a_node) const
  {
    const x::DOMNode* node    = a_node->getFirstChild();
    const auto        tag_hdr = "hdr";
    const auto        tag_q   = "q";
    gsql_q_set        q_set;
    while (node != nullptr)
    {
      if (node->getNodeType() == x::DOMNode::ELEMENT_NODE)
      {
        auto loc_name = toNative(node->getLocalName());
        err << out::sl(loc_name);
        if (loc_name == tag_hdr)
        { /* header */
          xercesc::DOMText* txt = static_cast<x::DOMText*>(node->getFirstChild());
          q_set.set_header(toNative(txt->getWholeText()));
        }
        else if (loc_name == tag_q) { /* query set */
          q_set = load_q(node, q_set);
        }
      }
      node = node->getNextSibling();
    };
    return q_set;
  }

  gsql_q_set core_parser::load_q(const x::DOMNode* a_node, gsql_q_set& q_set) const
  {
    auto node = a_node;
    while (node != nullptr)
    {
      if (node->getNodeType() == x::DOMNode::ELEMENT_NODE)
      {
        err << out::sl(std::string("query ")+toNative(node->getLocalName())); 
      }
      
       node = node->getNextSibling();
    }
    return q_set;
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
