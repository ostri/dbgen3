#ifndef CORE_PARSER_H
#define CORE_PARSER_H

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
    bool isValid() const { return valid_; }
    gsql_q_set parse_file(const str_t& a_file);
  private:
    gsql_q_set load_q_set(const x::DOMNode* a_node)const;
    gsql_q_set load_q(const x::DOMNode* a_node, gsql_q_set& q_set) const;
    bool g_init();
    /* .........................................................*/
    bool                     valid_;  //!< is instance valid ?
    bool                     init_;   //!< is initialization done?
    x::DOMImplementation*    impl_;   //!< implementation
    x::DOMLSParser*          parser_; //!< DOM parser
    std::unique_ptr<gsql_eh> eh_;     //!< error handler

  protected:
    gsql_q_set load_q_set(x::DOMNode* a_node) const;
  };
}; // namespace dbgen3

#endif
