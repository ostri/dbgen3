#ifndef CORE_PARSER_H
#define CORE_PARSER_H

#include <array>
#include <memory>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

#include "common.hpp"
#include "gsql_item_set.hpp"
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
    bool     handleError(const xercesc::DOMError& /*de*/) override
    {
      return false;
    };

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
    ///@}
    gsql_item_set parse_file(const str_t& a_file)
    {
      try
      {
        xercesc::DOMDocument* doc = nullptr;
        doc                       = parser_->parseURI(a_file.data());
        doc->release();
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

  private:
    bool g_init();
    /* .........................................................*/
    bool                        valid_;  //!< is instance valid ?
    bool                        init_;   //!< is initialization done?
    xercesc::DOMImplementation* impl_;   //!< implementation
    xercesc::DOMLSParser*       parser_; //!< DOM parser
    std::unique_ptr<gsql_eh>    eh_;     //!< error handler
  };
}; // namespace dbgen3

#endif
