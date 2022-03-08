#ifndef CORE_PARSER_HPP
#define CORE_PARSER_HPP

#include <array>
#include <filesystem>
#include <memory>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/framework/XMLGrammarPool.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/validators/common/Grammar.hpp>

#include "utility_classes/common.hpp"
#include "db_integration/db2_reader.hpp"
#include "gsql_parser/dom_error_handler.hpp"
#include "gsql_parser/gsql_q_set.hpp"
#include "gsql_parser/gsql_sql_set.hpp"
#include "gsql_parser/xerces_strings.hpp"
#include "runtime/statement.hpp"
#include "utility_classes/log.hpp"
#include "utility_classes/string_format.hpp"

namespace dbgen3
{
  namespace x = xercesc;
  class core_parser // NOLINT clang-tidy(cppcoreguidelines-special-member-functions)
  {
  public:
    explicit core_parser(x::XMLGrammarPool* gp);
    ~core_parser();
    core_parser(const core_parser&) = delete;
    core_parser& operator=(const core_parser&) = delete;
    core_parser& operator=(core_parser&&) = delete;
    /// @name getters
    ///@{
    static std::string q_set_id(const x::DOMElement* an_el, cstr_t a_filename);
    ///@}
    int        parse_set(const str_vec& gsql_files);
    gsql_q_set parse_file(cstr_t a_filename, const db2_reader& a_dbr, const RDBMS& a_db_type);
  protected:
  private:
    /// @name query set structure
    ///@{
    static fld_dscr load_fld_dscr(cstr_t def_name, const db::BUF_TYPE& a_bt, SQLHANDLE h, uint ndx);
    static fld_vec  fetch_param_dscr(const str_vec&      names,
                                     const db::BUF_TYPE& a_bt,
                                     db::statement&      a_stmt);
    static gsql_q_set load_q_set(const x::DOMElement* an_el,
                                 cstr_t               a_filename,
                                 const db2_reader&    a_dbr,
                                 const RDBMS&         a_db_type);
    static gsql_q     load_q(const x::DOMElement* an_el,
                             uint                 a_ndx,
                             str_vec              a_ctx,
                             const db2_reader&    a_dbr,
                             const RDBMS&         a_db_type);
    static gsql_q     load_q_children(const x::DOMElement* an_el,
                                      uint                 a_ndx,
                                      const str_vec&       a_ctx,
                                      const db2_reader&    a_dbr,
                                      const RDBMS&         a_db_type,
                                      gsql_q&              r);
    // static gsql_qbuf_dscr load_qp(const x::DOMElement* an_el, uint a_ndx);
    // static gsql_qbuf_dscr load_qr(const x::DOMElement* an_el, uint a_ndx);
    static gsql_qbuf_dscr load_buf(const db::BUF_TYPE&  bt,
                                   const x::DOMElement* an_el
                                   );
    static gsql_sql_set   load_sql_set(const x::DOMElement* an_el,
                                       const str_vec&       a_ctx,
                                       const RDBMS&         a_db_type);
    static gsql_sql_set   load_sql_set_sql(const x::DOMElement* el,
                                           str_vec              a_ctx,
                                           const RDBMS&         a_db_type,
                                           uint                 ndx,
                                           gsql_sql_set&        r);
    static gsql_q         load_q_sqlset_from_db(gsql_q& r, const db2_reader& a_dbr);
    ///@}

    static str_t attr_value(const x::DOMElement* an_el, cstr_t an_attr_name, cstr_t a_default);
    static bool  attr_value_b(const x::DOMElement* an_el, cstr_t an_attr_name, bool a_default);
    bool         init();
    static std::string get_statement(const x::DOMElement* an_el, str_vec a_ctx);
    static std::string get_text_node(const x::DOMElement* an_el, str_vec a_ctx);
    static std::pair<str_t, str_t> get_text_node_with_prepare(const x::DOMElement* an_el, str_vec a_ctx);
    /* .........................................................*/
    x::DOMLSParser*   parser_; //!< DOM parser
    dom_error_handler eh_{};   //!< error handler

  };
};     // namespace dbgen3
#endif // CORE_PARSER_HPP
