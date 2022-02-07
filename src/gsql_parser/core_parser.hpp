#ifndef CORE_PARSER_HPP
#define CORE_PARSER_HPP

#include <array>
#include <filesystem>
#include <memory>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

#include "common.hpp"
#include "db2_reader.hpp"
#include "gsql_q_set.hpp"
#include "gsql_sql_set.hpp"
#include "log.hpp"
#include "statement.hpp"
#include "string_format.hpp"
#include "xerces_strings.hpp"

namespace dbgen3
{
  namespace x = xercesc;

  class gsql_eh : public xercesc::DOMErrorHandler
  {
  public:
    gsql_eh()               = default;
    ~gsql_eh() override     = default;
    gsql_eh(const gsql_eh&) = delete;
    gsql_eh(gsql_eh&&)      = delete;
    gsql_eh& operator=(const gsql_eh&) = delete;
    gsql_eh& operator=(gsql_eh&&) = delete;
    bool     handleError(const xercesc::DOMError& /*de*/) override;
  private:
  };

  class core_parser
  {
  public:
    core_parser();
    ~core_parser();
    core_parser(const core_parser&) = delete;
    core_parser(core_parser&&)      = default;
    core_parser& operator=(const core_parser&) = delete;
    core_parser& operator=(core_parser&&) = delete;
    /// @name getters
    ///@{
    bool               isValid() const;
    static std::string q_set_id(const x::DOMElement* an_el, cstr_t a_filename);
    ///@}
    // static gsql_q_set parse_file(cstr_t a_filename, const db2_reader& a_dbr);
    int        parse_set(const str_vec& gsql_files);
    gsql_q_set parse_file(cstr_t a_filename, const db2_reader& a_dbr, const RDBMS& a_db_type);
  protected:
  private:
    /// @name query set structure
    ///@{
    static fld_dscr load_fld_dscr(const BUF_TYPE& a_bt, SQLHANDLE h, uint ndx);
    static fld_vec fetch_param_dscr(const BUF_TYPE& a_bt, db::statement& a_stmt);
    static gsql_q_set     load_q_set(const x::DOMElement* an_el,
                                     cstr_t               a_filename,
                                     const db2_reader&    a_dbr,
                                     const RDBMS&         a_db_type);
    static gsql_q         load_q(const x::DOMElement* an_el,
                                 uint                 a_ndx,
                                 str_vec              a_ctx,
                                 const db2_reader&    a_dbr,
                                 const RDBMS&         a_db_type);
    static gsql_q         load_q_children(const x::DOMElement* an_el,
                                          uint                 a_ndx,
                                          const str_vec&       a_ctx,
                                          const db2_reader&    a_dbr,
                                          const RDBMS&         a_db_type,
                                          gsql_q&              r);
    static gsql_qbuf_dscr load_qp(const x::DOMElement* an_el, uint a_ndx);
    static gsql_qbuf_dscr load_qr(const x::DOMElement* an_el, uint a_ndx);
    static gsql_sql_set   load_sql_set(const x::DOMElement* an_el,
                                       const str_vec&       a_ctx,
                                       const RDBMS&         a_db_type);
    static gsql_sql_set   load_sql_set_sql(const x::DOMElement* el,
                                           str_vec              a_ctx,
                                           const RDBMS&         a_db_type,
                                           uint                 ndx,
                                           gsql_sql_set&        r
                                           );
    static gsql_q         load_q_sqlset_from_db(gsql_q& r, const db2_reader& a_dbr);
    ///@}

    static std::string attr_value(const x::DOMElement* an_el,
                                  cstr_t               an_attr_name,
                                  cstr_t               a_default);
    static bool        attr_value(const x::DOMElement* an_el, cstr_t an_attr_name, bool a_default);
    bool               init();
    static std::string get_statement(const x::DOMElement* an_el, str_vec a_ctx);
    static std::string get_text_node(const x::DOMElement* an_el, str_vec a_ctx);
    /* .........................................................*/
    bool                     valid_;  //!< is instance valid ?
    bool                     init_;   //!< is initialization done?
    x::DOMImplementation*    impl_;   //!< implementation
    x::DOMLSParser*          parser_; //!< DOM parser
    std::unique_ptr<gsql_eh> eh_;     //!< error handler


  };

}; // namespace dbgen3

#endif // CORE_PARSER_HPP
