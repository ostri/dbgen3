#include <filesystem>
#include <xercesc/dom/DOMText.hpp>

#include "core_parser.hpp"
#include "xerces_strings.hpp"
#include "gsql_q_set.hpp"
#include "gsql_q.hpp"


namespace dbgen3
{
  const XS tag_id{u"id"};
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
  bool core_parser::g_init()
  {
    if (! init_)
    {
      xercesc::XMLPlatformUtils::Initialize();
      init_ = true;
    }
    return init_;
  }
  gsql_q_set core_parser::parse_file(cstr_t a_file)
  {
    try
    {
      auto doc = parser_->parseURI(a_file.data());
      if (doc != nullptr)
      {
        const x::DOMElement* root = doc->getDocumentElement();
        auto                 y    = load_q_set(root, a_file);
        err << y.dump(" ", 0);
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
    catch (const std::runtime_error& e)
    {
      throw;
    }
    catch (...)
    {
      err << out::sl("parser unknown exception", 0);
    };
    return {};
  }
  std::string core_parser::attr_value(const x::DOMElement* a_node,
                                      cstr_t   an_attr_name,
                                      cstr_t   a_default)
  {
    XS attr_id = a_node->getAttribute(fromNative(str_t(an_attr_name)).data());
    if (! attr_id.empty()) return toNative(attr_id);
    else return std::string(a_default);
  }
  bool core_parser::attr_value(const x::DOMElement* a_node,
                               const std::string&   an_attr_name,
                               bool                 a_default)
  {
    /// load attributes
    XS attr_id(a_node->getAttribute(fromNative(an_attr_name).data()));
    if (! attr_id.empty())
    {
      if (toNative(attr_id) == "false") return false;
      else return true;
    }
    else return a_default;
  }
  gsql_qbuf_dscr core_parser::load_qp(const x::DOMElement* a_node, uint a_ndx)
  {
    gsql_qbuf_dscr r(gsql_qbuf_dscr::type::par);
    /// load attributes
    r.set_id(attr_value(a_node, "id", "qp_" + std::to_string(a_ndx)));
    r.set_skip(attr_value(a_node, "skip", false));
    return r;
  }
  gsql_qbuf_dscr core_parser::load_qr(const x::DOMElement* a_node, uint a_ndx)
  {
    gsql_qbuf_dscr r(gsql_qbuf_dscr::type::res);
    /// load attributes
    r.set_id(attr_value(a_node, "id", "qp_" + std::to_string(a_ndx)));
    r.set_skip(attr_value(a_node, "skip", false));
    return r;
  }

  gsql_q core_parser::load_q(const x::DOMElement* a_node, uint a_ndx) const
  {
    gsql_q      r; /// result
    x::DOMNode* node = a_node->getFirstChild();
    assert(node != nullptr); // query has at least sql statement
    /// load attributes
    r.set_id(attr_value(a_node, "id", "q_" + std::to_string(a_ndx))); // load unique query id
    auto ndx_str = std::to_string(a_ndx);
    auto b_par   = gsql_qbuf_dscr(gsql_qbuf_dscr::type::par, "qp_" + ndx_str, true);
    auto b_res   = gsql_qbuf_dscr(gsql_qbuf_dscr::type::res, "qr_" + ndx_str, true);
    r.set_buf_dscr(b_par);
    r.set_buf_dscr(b_res);
    /// load children
    while (node != nullptr)
    {
      if (node->getNodeType() == x::DOMNode::ELEMENT_NODE)
      {
        auto el       = static_cast<x::DOMElement*>(node);
        auto loc_name = toNative(node->getLocalName());
        if (loc_name == "qp") r.set_buf_dscr(load_qp(el, a_ndx));
        else if (loc_name == "qr") r.set_buf_dscr(load_qr(el, a_ndx));
        else if (loc_name == "sql-set") { /*TODO*/
        }
        else
          throw std::runtime_error(std::string("internal error ") + loc_name + " " +
                                   std::string(__FILE__) + " " + std::to_string(__LINE__));
        a_ndx++;
      }
      else if (node->getNodeType() == x::DOMNode::COMMENT_NODE) {
        // comments are skipped
      }
      else if (node->getNodeType() == x::DOMNode::TEXT_NODE) {
        // ignorable whitespace is ignored
      }
      else err << out::sl(std::string("unhandled: ") + std::to_string(node->getNodeType()));
      node = node->getNextSibling();
    }
    return r;
  }
  std::string core_parser::g_id(const x::DOMElement* a_node, cstr_t a_filename) const
  {
    return attr_value(a_node, "id", fs::path(a_filename).stem().string());
  }

  gsql_q_set core_parser::load_q_set(const x::DOMElement* a_node,
                                     cstr_t   a_filename) const
  {
    assert(a_node != nullptr);
    const XS   tag_hdr{u"hdr"};
    const XS   tag_q{u"q"};
    gsql_q_set q_set;
    q_set.set_id(g_id(a_node, a_filename));

    const x::DOMNode* node  = a_node->getFirstChild();
    auto              q_ndx = 0;
    while (node != nullptr)
    {
      if (node->getNodeType() == x::DOMNode::ELEMENT_NODE)
      {
        XS loc_name = node->getLocalName();
        //        err << out::sl(toNative(loc_name));
        if (loc_name == tag_hdr)
        { /* header */
          auto txt = static_cast<x::DOMText*>(node->getFirstChild());
          q_set.set_header(toNative(txt->getWholeText()));
        }
        else if (loc_name == tag_q) { /* query set */
          err << out::sl(toNative(loc_name));
          auto q_result = load_q(static_cast<const x::DOMElement*>(node), ++q_ndx);
          auto sts      = q_set.q_insert(q_result);
          err << " status: " << sts << std::endl;
        }
      }
      node = node->getNextSibling();
    };
    return q_set;
  }
} // namespace dbgen3
