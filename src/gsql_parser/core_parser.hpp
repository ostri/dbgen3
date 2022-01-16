#ifndef CORE_PARSER_HPP
#define CORE_PARSER_HPP

#include <filesystem>
#include <array>
#include <memory>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

#include "common.hpp"
#include "gsql_q_set.hpp"
#include "log.hpp"
#include "string_format.hpp"
#include "xerces_strings.hpp"
#include "gsql_sql_set.hpp"
namespace dbgen3
{
  namespace x  = xercesc;
  namespace fs = std::filesystem;

  class gsql_eh : public xercesc::DOMErrorHandler
  {
  public:
    gsql_eh()               = default;
    ~gsql_eh() override     = default;
    gsql_eh(const gsql_eh&) = delete;
    gsql_eh(gsql_eh&&)      = delete;
    gsql_eh& operator=(const gsql_eh&) = delete;
    gsql_eh& operator=(gsql_eh&&) = delete;
    bool     handleError(const xercesc::DOMError& /*de*/) override { return false; };
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
    bool        isValid() const;
    std::string g_id(const x::DOMElement* an_el, cstr_t a_filename) const;
    ///@}
    gsql_q_set parse_file(cstr_t a_file);
  private:
    /// @name query set structure
    ///@{
    gsql_q_set            load_q_set(const x::DOMElement* an_el, cstr_t a_filename) const;
    static gsql_q         load_q(const x::DOMElement* an_el, uint a_ndx);
    static gsql_qbuf_dscr load_qp(const x::DOMElement* an_el, uint a_ndx);
    static gsql_qbuf_dscr load_qr(const x::DOMElement* an_el, uint a_ndx);
    static gsql_sql_set   load_sql_set(const x::DOMElement* an_el);
    ///@}
    static std::string attr_value(const x::DOMElement* a_node,
                                  cstr_t               an_attr_name,
                                  cstr_t               a_default);
    static bool        attr_value(const x::DOMElement* a_node,
                                  const std::string&   an_attr_name,
                                  bool                 a_default);
    bool               g_init();
    /* .........................................................*/
    bool                     valid_;  //!< is instance valid ?
    bool                     init_;   //!< is initialization done?
    x::DOMImplementation*    impl_;   //!< implementation
    x::DOMLSParser*          parser_; //!< DOM parser
    std::unique_ptr<gsql_eh> eh_;     //!< error handler
  };

  inline bool core_parser::isValid() const { return valid_; }
}; // namespace dbgen3

#endif // CORE_PARSER_HPP
