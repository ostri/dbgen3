#include <filesystem>
#include <stdexcept>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/framework/XMLGrammarPool.hpp>

#include "core_parser.hpp"
#include "db2_reader.hpp"
#include "enums.hpp"
#include "exceptions.hpp"
#include "executor.hpp"
#include "gsql_q.hpp"
#include "gsql_q_set.hpp"
#include "program_status.hpp"
#include "xerces_strings.hpp"

namespace dbgen3
{
  namespace fs = std::filesystem;
  namespace x  = xercesc;
  core_parser::core_parser(x::XMLGrammarPool* gp)
  //  : valid_(false)
  //  , impl_(xercesc::DOMImplementationRegistry::getDOMImplementation(u"LS"))
  // , parser_(static_cast<xercesc::DOMImplementationLS*>(impl_) //
  //             ->createLSParser(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, nullptr))
  : parser_(executor::create_parser(gp))
  //  , eh_(new gsql_eh())
  //  , gp_(gp)
  {
    try
    {
      //      valid_  = true;
      //    error_handler eh;
      parser_->getDomConfig()->setParameter(x::XMLUni::fgDOMErrorHandler, &eh_); // NOLINT
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
      //      parser_ = nullptr;
    }
  }
  //  bool core_parser::isValid() const { return valid_; }


  gsql_qbuf_dscr core_parser::load_qp(const x::DOMElement* an_el, uint a_ndx)
  {
    gsql_qbuf_dscr r(db::BUF_TYPE::par);
    /// load attributes
    r.set_id(attr_value(an_el, "id", "qp_" + std::to_string(a_ndx)));
    r.set_skip(attr_value(an_el, "skip", false));
    return r;
  }
  gsql_qbuf_dscr core_parser::load_qr(const x::DOMElement* an_el, uint a_ndx)
  {
    gsql_qbuf_dscr r(db::BUF_TYPE::res);
    /// load attributes
    r.set_id(attr_value(an_el, "id", "qp_" + std::to_string(a_ndx)));
    r.set_skip(attr_value(an_el, "skip", false));
    return r;
  }

  std::string core_parser::get_text_node(const x::DOMElement* an_el, str_vec a_ctx)
  {
    std::string r;
    for (auto* n(an_el->getFirstChild()); n != nullptr; n = n->getNextSibling())
    {
      auto n_type = static_cast<NT>(n->getNodeType()); // tag type
      switch (n_type)
      {
      case NT::TEXT_NODE:
      case NT::CDATA_SECTION_NODE:
      {
        const auto* txt = dynamic_cast<const x::DOMText*>(n);
        uint64_t    len = txt->getLength();
        if (len >= r.max_size())
        {
          auto max_len = r.max_size();
          auto msg     = fmt::format(fg(fmt::color::crimson),
                                 dbgen3::program_status::dscr(P_STS::sql_stat_too_long),
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
  gsql_sql_set core_parser::load_sql_set_sql(const x::DOMElement* el,
                                             str_vec              a_ctx,
                                             const RDBMS&         a_db_type,
                                             uint                 ndx,
                                             gsql_sql_set&        r)
  {
    // const auto* el = dynamic_cast<const x::DOMElement*>(n);
    //  no enum value testing; XSD handles that
    auto rdbms = ME::enum_cast<RDBMS>(attr_value(el, "rdbms", ME::enum_name(RDBMS::sql)));
    if ((rdbms == RDBMS::sql) || (rdbms == a_db_type))
    { /// only generic SQLs or for a specific database type
      auto phase = ME::enum_cast<PHASE>(attr_value(el, "phase", ME::enum_name(PHASE::main)));
      const std::string stmt = get_text_node(el, a_ctx);
      gsql_sql_dscr     sql_dscr(rdbms.value(), phase.value(), stmt);
      auto              res = r.insert(sql_dscr);
      if (res == INS_OP::ambigous)
      { // the same specific database twice
        // auto ctx = ctx_to_str(a_ctx, std::to_string(sql_cnt));
        auto msg = fmt::format(fg(fmt::color::crimson),
                               dbgen3::program_status::dscr(P_STS::duplicate_sql_buf_def),
                               a_ctx[1],
                               ndx,
                               ME::enum_name<RDBMS>(rdbms.value()),
                               ME::enum_name<PHASE>(phase.value()));
        throw dbgen3_exc(P_STS::duplicate_sql_buf_def, msg);
      }
    }
    return r;
  };
  /**
   * @brief contents of <sql-set> and below store in internal structure
   *
   * @param an_el         element whre sql-set stars
   * @param a_ctx         context (for error reporting)
   * @param a_db_type     type of the database
   * @return gsql_sql_set sql-set stored in internal stucture
   */
  gsql_sql_set core_parser::load_sql_set(const x::DOMElement* an_el,
                                         const str_vec&       a_ctx,
                                         const RDBMS&         a_db_type)
  {
    gsql_sql_set r(a_db_type); //!< result of the method
    /// loop over all subordinated SQL elements
    auto sql_cnt = 0;
    for (auto* n(an_el->getFirstChild()); n != nullptr; n = n->getNextSibling())
    {
      auto        n_type = static_cast<NT>(n->getNodeType()); // tag type
      const auto* ln     = n->getLocalName();
      auto        l_name = (ln != nullptr) ? toNative(ln) : ""; // tag name in utf8
      // err << fmt::format("local name '{}' node type '{}'", l_name, ME::enum_name(n_type));
      switch (n_type)
      {
      case NT::ELEMENT_NODE:
      {
        if (l_name == "sql")
        { /* only sql tag is allowed */
          ++sql_cnt;
          const auto* el = dynamic_cast<const x::DOMElement*>(n);
          r              = load_sql_set_sql(el, a_ctx, a_db_type, sql_cnt, r);
        }
        else err << out::sl(fmt::format("unknown tag within sql: query id:'{}'", l_name));
        break;
      }
      case NT::TEXT_NODE:              /// we skip TEXT_NODES and COMMENT_NODES
      case NT::COMMENT_NODE: continue; /// text nodes are ignorable whitespace
      default:
      {
        throw std::runtime_error(
          fmt::format("Should be ELEMENT_NODE, TEXT_NOD or COMMENT_NODE, it was {} ctx: {}",
                      ME::enum_name<NT>(n_type),
                      ctx_to_str(a_ctx)));
      }
      }
    }
    return r;
  }
  std::string core_parser::attr_value(const x::DOMElement* an_el,
                                      cstr_t               an_attr_name,
                                      cstr_t               a_default)
  {
    XS attr_id = an_el->getAttribute(fromNative(str_t(an_attr_name)).data());
    if (! attr_id.empty()) return toNative(attr_id);
    return std::string(a_default);
  }
  bool core_parser::attr_value(const x::DOMElement* an_el, cstr_t an_attr_name, bool a_default)
  {
    /// load attributes
    XS attr_id(an_el->getAttribute(fromNative(an_attr_name.data()).data()));
    if (! attr_id.empty()) { return toNative(attr_id) == "true"; }
    return a_default;
  }
  fld_dscr core_parser::load_fld_dscr(const db::BUF_TYPE& a_bt, SQLHANDLE h, uint ndx)
  {
    int16_t    type;
    uint32_t   width;
    int16_t    dec;
    int16_t    nullable;
    int16_t    col_name_len           = 0;
    const auto max_coloum_name_length = 50;
    int16_t    rc;

    std::array<char, max_coloum_name_length + 1> buf{}; /// buffer that
                                                        /// returns the name of the column

    rc =
      (a_bt == db::BUF_TYPE::par)
        ? SQLDescribeParam(h, ndx + 1, &type, &width, &dec, &nullable)
        : SQLDescribeCol(h,
                         ndx + 1,
                         // NOLINTNEXTLINE clang-tidy(cppcoreguidelines-pro-type-reinterpret-cast)
                         reinterpret_cast<unsigned char*>(buf.data()),
                         buf.size(),
                         &col_name_len,
                         &type,
                         &width,
                         &dec,
                         &nullable);
    if (SQL_SUCCESS == rc)
    { /// FIXME convert to to_char
      std::string name = (a_bt == db::BUF_TYPE::par) ? "par_" + std::to_string(ndx + 1)
                                                     : std::string(buf.data(), col_name_len);
      for (auto& ch : name)
        ch = static_cast<char>(std::tolower(ch)); // FIXME dosn't work with utf-8

      auto n  = ME::enum_cast<DBN>(nullable);
      auto ot = ME::enum_cast<ODBC_TYPE>(type);
      if (! ot.has_value())
        throw std::runtime_error("Unsupported column type " + std::to_string(type));
      info << fmt::format("[{:3}]: name: {:24} type:{:4} {:14} width:{:5} dec:{:3} null:{:16}",
                          ndx + 1,
                          name,
                          ME::enum_integer(ot.value()),
                          ME::enum_name(ot.value()),
                          width,
                          dec,
                          ME::enum_name<DBN>(n.value()));
      fld_dscr fld(ndx + 1, name, ot.value(), width, dec, n.value());
      return fld;
    };
    throw db::error_exception(db::error(h, SQL_HANDLE_STMT));
  }

  /**
   * @brief it fetches the parameter/column description or throws exception
   *
   * @param a_bt bufer_type
   * @param a_stmt statement structure we are executing statements upon
   * @return fld_vec
   */
  fld_vec core_parser::fetch_param_dscr(const db::BUF_TYPE& a_bt, db::statement& a_stmt)
  {
    fld_vec r;
    auto    h       = a_stmt.handle();
    auto    rc      = SQL_SUCCESS;
    int16_t par_cnt = 0;
    rc = (a_bt == db::BUF_TYPE::par) ? SQLNumParams(h, &par_cnt) : SQLNumResultCols(h, &par_cnt);
    if (SQL_SUCCESS == rc)
    {
      info << "# of par:" << par_cnt << "\n";
      for (auto cnt = 0; cnt < par_cnt; ++cnt) { r.push_back(load_fld_dscr(a_bt, h, cnt)); };
    }
    else throw db::error_exception(db::error(h, SQL_HANDLE_STMT));
    return r;
  }
  gsql_q core_parser::load_q_sqlset_from_db(gsql_q& r, const db2_reader& a_dbr)
  {
    // TODO(ostri): think if we need only prepare, main and no cleanup, since we have rollback
    auto sql_m = r.sql(PHASE::main);
    auto sql_p = r.sql(PHASE::prepare); // preparation
    auto sql_c = r.sql(PHASE::cleanup); // cleanup
    if (! sql_m.empty())
    {
      auto          rc = SQL_SUCCESS;
      db::statement stmt_m((a_dbr.connection()));
      db::statement stmt_p((a_dbr.connection()));
      db::statement stmt_c((a_dbr.connection()));
      if ((rc == SQL_SUCCESS) && ! sql_p.empty()) rc = stmt_p.exec_direct(sql_p, true);
      rc = stmt_m.prepare(sql_m);
      r.set_buf_dscr_flds(db::BUF_TYPE::par, fetch_param_dscr(db::BUF_TYPE::par, stmt_m));
      r.set_buf_dscr_flds(db::BUF_TYPE::res, fetch_param_dscr(db::BUF_TYPE::res, stmt_m));
      if ((rc == SQL_SUCCESS) && ! sql_c.empty()) rc = stmt_c.exec_direct(sql_c, true);
      //            err << sts;
      stmt_p.rollback();
      stmt_m.rollback();
      stmt_c.rollback();
    }
    else {
      // FIXME what we should do? abort or detect the missing sql later?
    }
    return r;
  };
  gsql_q core_parser::load_q_children(const x::DOMElement* an_el,
                                      uint                 a_ndx,
                                      const str_vec&       a_ctx,
                                      const db2_reader&    a_dbr,
                                      const RDBMS&         a_db_type,
                                      gsql_q&              r)
  {
    for (x::DOMNode* n(an_el->getFirstChild()); n != nullptr; n = n->getNextSibling())
    {
      auto n_type = static_cast<NT>(n->getNodeType()); // tag type
      switch (n_type)
      {
      case NT::ELEMENT_NODE:
      {
        auto* el       = dynamic_cast<x::DOMElement*>(n);
        auto  loc_name = toNative(n->getLocalName());
        if (loc_name == "qp") r.set_buf_dscr(load_qp(el, a_ndx));
        else if (loc_name == "qr") r.set_buf_dscr(load_qr(el, a_ndx));
        else if (loc_name == "sql-set") //
        {
          r.set_sql_set(load_sql_set(el, a_ctx, a_db_type)); // XML(DOM) -> internal stuctures
          r = load_q_sqlset_from_db(r, a_dbr);
        }
        else
          throw std::runtime_error(std::string("internal error only qp, qr and sql-set allowed.") +
                                   loc_name + " " + std::string(__FILE__) + " " +
                                   std::to_string(__LINE__));
        a_ndx++;
      }
      case NT::TEXT_NODE:
      case NT::COMMENT_NODE: continue; // we skip ignorable WS and comments
      default:
      {
        throw std::runtime_error(
          fmt::format("Should be ELEMENT_NODE, TEXT_NOD or COMMENT_NODE, but it is '{}' ctx: {}",
                      ME::enum_name<NT>(n_type),
                      ctx_to_str(a_ctx)));
      }
      }
    }
    return r;
  }
  gsql_q core_parser::load_q(const x::DOMElement* an_el,
                             uint                 a_ndx,
                             str_vec              a_ctx,
                             const db2_reader&    a_dbr,
                             const RDBMS&         a_db_type)
  {
    gsql_q r(a_db_type); /// result
    /// load attributes
    r.set_id(attr_value(an_el, "id", "q_" + std::to_string(a_ndx))); // load unique query id
    a_ctx.emplace_back(std::string(r.id()));
    auto ndx_str = std::to_string(a_ndx);
    // implicit values unless explicit are provided
    r.set_buf_dscr(gsql_qbuf_dscr(db::BUF_TYPE::par, "qp_" + ndx_str, false));
    r.set_buf_dscr(gsql_qbuf_dscr(db::BUF_TYPE::res, "qr_" + ndx_str, false));
    r = load_q_children(an_el, a_ndx, a_ctx, a_dbr, a_db_type, r);
    return r;
  }
  std::string core_parser::q_set_id(const x::DOMElement* an_el, cstr_t a_filename)
  {
    return attr_value(an_el, "id", fs::path(a_filename).stem().string());
  }
  /**
   * @brief parse gsql file and build internal gsql structure (set of query definitiosn)
   *
   * @param a_filename name of the gsql file
   * @param a_dbr access to the db2 database
   * @param a_db_type type of the database
   * @return gsql_q_set data structure extracted from the gsql /xml file
   */
  gsql_q_set core_parser::parse_file(cstr_t            a_filename,
                                     const db2_reader& a_dbr,
                                     const RDBMS&      a_db_type)
  {
    try
    {
      std::error_code ec;
      fs::path        path(a_filename);
      auto            fn = fs::weakly_canonical(path, ec).string();
      // auto fn = a_filename;
      if (file_exists(fn))
      {
        auto* doc = parser_->parseURI(fn.data());
        if (! eh_.failed()) return load_q_set(doc->getDocumentElement(), fn, a_dbr, a_db_type);
        auto msg = fmt::format(fg(fmt::color::crimson),
                                dbgen3::program_status::dscr(P_STS::inv_gsql_syntax).data(),
                                eh_.uri(),
                                eh_.line(),
                                eh_.col(),
                                eh_.e_type(),
                                eh_.e_msg());
        throw dbgen3_exc(P_STS::inv_gsql_syntax, msg);
      }
      auto msg = fmt::format(fg(fmt::color::crimson), //
                             dbgen3::program_status::dscr(P_STS::gsql_file_not_exists),
                             fn);
      throw dbgen3_exc(P_STS::gsql_file_not_exists, msg);
    }
    catch (const x::XMLException& e)
    {
      err << out::sl("Exception message is: ", toNative(e.getMessage()), 0);
    }
    catch (const x::DOMException& e)
    {
      err << out::sl("Exception message is: ", toNative(e.getMessage()), 0);
    }
    catch (const dbgen3_exc& e)
    {
//      err << __FILE__ << __LINE__ << e.what() << "\n";
      throw;
    }
    catch (const std::runtime_error& e)
    {
      err << __FILE__ << __LINE__ << e.what() << "\n";
      throw;
    }
    catch (...)
    {
      err << out::sl("parser unknown exception", 0);
    };
    return {};
  }
  /**
   * @brief load query set from DOM tree
   *
   * @param an_el       DOM element that holds "q-set" structure
   * @param a_filename  filename where this xml is held
   * @param a_ctx
   * @return gsql_q_set
   */
  gsql_q_set core_parser::load_q_set(const x::DOMElement* an_el,
                                     cstr_t               a_filename,
                                     const db2_reader&    a_dbr,
                                     const RDBMS&         a_db_type)
  {
    assert(an_el != nullptr); // NOLINT clang-tidy(hicpp-no-array-decay)
    str_vec    ctx;           // we are top level structure, therefore context is empty;
    gsql_q_set q_set;
    q_set.set_id(q_set_id(an_el, a_filename));
    ctx.emplace_back(q_set.id());
    auto q_ndx = 0;
    for (x::DOMNode* n = an_el->getFirstChild(); n != nullptr; n = n->getNextSibling())
    {
      if (n->getNodeType() == x::DOMNode::ELEMENT_NODE)
      {
        auto* el       = dynamic_cast<x::DOMElement*>(n);
        auto  loc_name = toNative(n->getLocalName());
        if (loc_name == "hdr") q_set.set_header(get_text_node(el, ctx)); /* header */
        else if (loc_name == "q") {                                      /* query set */
          auto sts = q_set.q_insert(load_q(el, ++q_ndx, ctx, a_dbr, a_db_type));
          if (! sts)
          {
            auto msg = fmt::format(fg(fmt::color::crimson),
                                   dbgen3::program_status::dscr(P_STS::duplicate_sql_dscr_def),
                                   q_set.id());
            throw dbgen3_exc(P_STS::duplicate_sql_dscr_def, msg);
          }
        }
      }
    };
    return q_set;
  }
  // bool gsql_eh::handleError(const xercesc::DOMError& /*de*/)
  // {
  //   return false;
  // };
} // namespace dbgen3
