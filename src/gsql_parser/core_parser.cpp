#include <filesystem>
#include <xercesc/dom/DOMText.hpp>

#include "core_parser.hpp"
#include "xerces_strings.hpp"
#include "gsql_q_set.hpp"
#include "gsql_q.hpp"
#include "exceptions.hpp"
#include "enums.hpp"

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
      auto c = parser_->getDomConfig();
      // c->setParameter(x::XMLUni::fgDOMValidate,
      //                                       true);
      c->setParameter(x::XMLUni::fgDOMNamespaces, true);
      c->setParameter(x::XMLUni::fgDOMDatatypeNormalization, true);
      /// error handler
      c->setParameter(xercesc::XMLUni::fgDOMErrorHandler, eh_.get());
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
    if (! init_) xercesc::XMLPlatformUtils::Initialize();
    init_ = true;
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
      err << __FILE__ << __LINE__ << "\n";
      throw;
    }
    catch (...)
    {
      err << out::sl("parser unknown exception", 0);
    };
    return {};
  }
  std::string core_parser::attr_value(const x::DOMElement* an_el,
                                      cstr_t               an_attr_name,
                                      cstr_t               a_default)
  {
    XS attr_id = an_el->getAttribute(fromNative(str_t(an_attr_name)).data());
    if (! attr_id.empty()) return toNative(attr_id);
    else return std::string(a_default);
  }
  bool core_parser::attr_value(const x::DOMElement* an_el,
                               const std::string&   an_attr_name,
                               bool                 a_default)
  {
    /// load attributes
    XS attr_id(an_el->getAttribute(fromNative(an_attr_name).data()));
    if (! attr_id.empty())
    {
      if (toNative(attr_id) == "false") return false;
      else return true;
    }
    else return a_default;
  }
  gsql_qbuf_dscr core_parser::load_qp(const x::DOMElement* an_el, uint a_ndx)
  {
    gsql_qbuf_dscr r(BUF_TYPE::par);
    /// load attributes
    r.set_id(attr_value(an_el, "id", "qp_" + std::to_string(a_ndx)));
    r.set_skip(attr_value(an_el, "skip", false));
    return r;
  }
  gsql_qbuf_dscr core_parser::load_qr(const x::DOMElement* an_el, uint a_ndx)
  {
    gsql_qbuf_dscr r(BUF_TYPE::res);
    /// load attributes
    r.set_id(attr_value(an_el, "id", "qp_" + std::to_string(a_ndx)));
    r.set_skip(attr_value(an_el, "skip", false));
    return r;
  }

  std::string core_parser::get_text_node(const x::DOMElement* an_el, str_vec a_ctx)
  {
    std::string r;
    for (auto n(an_el->getFirstChild()); n != nullptr; n = n->getNextSibling())
    {
      auto n_type = static_cast<NT>(n->getNodeType()); // tag type
      switch (n_type)
      {
      case NT::TEXT_NODE:
      case NT::CDATA_SECTION_NODE:
      {
        auto     txt = static_cast<const x::DOMText*>(n);
        uint64_t len = txt->getLength();
        if (len >= r.max_size())
        {
          auto max_len = r.max_size();
          auto msg     = fmt::format(fg(fmt::color::crimson),
                                 program_status().dscr(P_STS::sql_stat_too_long),
                                 max_len,
                                 a_ctx[1],
                                 a_ctx[2]);
          throw dbgen3_exc(P_STS::sql_stat_too_long, msg);
        }
        XS tmp = txt->substringData(0, len);
        r += toNative(tmp);
        break;
      }
      case NT::ELEMENT_NODE: throw std::runtime_error("mixed contents");
      case NT::COMMENT_NODE: continue;
      default:
      {
        throw std::runtime_error(fmt::format(
          "Should be DOMText, it was {} ctx: {}", ME::enum_name<NT>(n_type), ctx_to_str(a_ctx)));
      }
      }
    }
    return r;
  }
  gsql_sql_set core_parser::load_sql_set(const x::DOMElement* an_el, str_vec a_ctx)
  {
    gsql_sql_set r; //!< result of the method
    /// loop over all subordinated SQL elements
    auto sql_cnt = 0;
    for (x::DOMNode* n = an_el->getFirstChild(); n != nullptr; n = n->getNextSibling())
    {
      auto n_type = static_cast<NT>(n->getNodeType()); // tag type
      auto ln     = n->getLocalName();
      auto l_name = (ln != nullptr) ? toNative(ln) : ""; // tag name in utf8
      //      err << fmt::format("local name '{}' node type '{}'", l_name, ME::enum_name(n_type));
      if (n_type == NT::ELEMENT_NODE)
      {
        if (l_name == "sql")
        { /* only sql tag is allowed */
          ++sql_cnt;
          auto el = static_cast<const x::DOMElement*>(n);
          // no enum value testing; XSD handles that
          auto rdbms = ME::enum_cast<RDBMS>(attr_value(el, "rdbms", ME::enum_name(RDBMS::db2)));
          auto phase = ME::enum_cast<PHASE>(attr_value(el, "phase", ME::enum_name(PHASE::main)));
          const std::string stmt = get_text_node(el, a_ctx);
          gsql_sql_dscr     sql_dscr(rdbms.value(), phase.value(), stmt);
          if (! r.insert(sql_dscr))
          {
            // auto ctx = ctx_to_str(a_ctx, std::to_string(sql_cnt));
            auto msg = fmt::format(fg(fmt::color::crimson),
                                   program_status().dscr(P_STS::duplicate_sql_buf_def),
                                   a_ctx[1],
                                   sql_cnt,
                                   ME::enum_name<RDBMS>(rdbms.value()),
                                   ME::enum_name<PHASE>(phase.value()));
            throw dbgen3_exc(P_STS::duplicate_sql_buf_def, msg);
          }
        }
        else err << out::sl(fmt::format("unknown tag within sql: query id:'{}'", l_name));
      }
      else if (n_type == NT::COMMENT_NODE) { /* comments are skipped */
      }
      else if (n_type == NT::TEXT_NODE) { /* ignorable whitespace is ignored */
      }
      else err << out::sl(std::string("unhandled: ") + str_t(ME::enum_name(n_type)) + " " + l_name);
    }
    return r;
  }
  gsql_q core_parser::load_q(const x::DOMElement* an_el, uint a_ndx, str_vec a_ctx)
  {
    gsql_q r; /// result
    /// load attributes
    r.set_id(attr_value(an_el, "id", "q_" + std::to_string(a_ndx))); // load unique query id
    a_ctx.emplace_back(std::string(r.id()));
    auto ndx_str = std::to_string(a_ndx);
    // implicit values unless explicit are provided
    r.set_buf_dscr(gsql_qbuf_dscr(BUF_TYPE::par, "qp_" + ndx_str, true));
    r.set_buf_dscr(gsql_qbuf_dscr(BUF_TYPE::res, "qr_" + ndx_str, true));
    /// load children
    for (x::DOMNode* n = an_el->getFirstChild(); n != nullptr; n = n->getNextSibling())
    {
      if (n->getNodeType() == x::DOMNode::ELEMENT_NODE)
      {
        auto el       = static_cast<x::DOMElement*>(n);
        auto loc_name = toNative(n->getLocalName());
        if (loc_name == "qp") r.set_buf_dscr(load_qp(el, a_ndx));
        else if (loc_name == "qr") r.set_buf_dscr(load_qr(el, a_ndx));
        else if (loc_name == "sql-set") {
          r.set_sql_set(load_sql_set(el, a_ctx));
        }
        else
          throw std::runtime_error(std::string("internal error only qp, qr and sql-set allowed.") +
                                   loc_name + " " + std::string(__FILE__) + " " +
                                   std::to_string(__LINE__));
        a_ndx++;
      }
      else if (n->getNodeType() == x::DOMNode::COMMENT_NODE) {
        // comments are skipped
      }
      else if (n->getNodeType() == x::DOMNode::TEXT_NODE) {
        // ignorable whitespace is ignored
      }
      else err << out::sl(std::string("unhandled: ") + std::to_string(n->getNodeType()));
    }
    return r;
  }
  std::string core_parser::q_set_id(const x::DOMElement* an_el, cstr_t a_filename)
  {
    return attr_value(an_el, "id", fs::path(a_filename).stem().string());
  }
  /**
   * @brief load query set from DOM tree
   *
   * @param an_el       DOM element that holds "q-set" structure
   * @param a_filename  filename where this xml is held
   * @param a_ctx
   * @return gsql_q_set
   */
  gsql_q_set core_parser::load_q_set(const x::DOMElement* an_el, cstr_t a_filename)
  {
    assert(an_el != nullptr);
    str_vec    ctx; // we are top level structure, therefore context is empty;
    gsql_q_set q_set;
    q_set.set_id(q_set_id(an_el, a_filename));
    ctx.emplace_back(q_set.id());
    auto q_ndx = 0;
    for (x::DOMNode* n = an_el->getFirstChild(); n != nullptr; n = n->getNextSibling())
    {
      if (n->getNodeType() == x::DOMNode::ELEMENT_NODE)
      {
        auto el       = static_cast<x::DOMElement*>(n);
        auto loc_name = toNative(n->getLocalName());
        if (loc_name == "hdr") q_set.set_header(get_text_node(el, ctx)); /* header */
        else if (loc_name == "q") {                                      /* query set */
          auto sts = q_set.q_insert(load_q(el, ++q_ndx, ctx));
          if (! sts)
          {
            auto msg = fmt::format(fg(fmt::color::crimson),
                                   program_status().dscr(P_STS::duplicate_sql_dscr_def),
                                   q_set.id());
            throw dbgen3_exc(P_STS::duplicate_sql_dscr_def, msg);
          }
        }
      }
    };
    return q_set;
  }
} // namespace dbgen3
