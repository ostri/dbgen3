#ifndef GSQL_PARSER_H
#define GSQL_PARSER_H

#include <array>
#include <memory>

// #include <xercesc/dom/DOM.hpp>
// #include <xercesc/dom/DOMErrorHandler.hpp>
// #include <xercesc/util/PlatformUtils.hpp>
// #include <xercesc/util/XMLString.hpp>

#include "common.hpp"
#include "core_parser.hpp"
#include "log.hpp"
#include "string_format.hpp"

namespace dbgen3
{

  class gsql_parser
  {
  public:
    gsql_parser();
    ~gsql_parser()                  = default;
    gsql_parser(const gsql_parser&) = delete;
    gsql_parser(gsql_parser&&)      = default;
    gsql_parser& operator=(const gsql_parser&) = delete;
    gsql_parser& operator=(gsql_parser&&) = delete;
    /// @name getters
    ///@{
    bool isValid() const { return valid_; }
    ///@}

  private:
    bool        valid_;  //!< is instance valid ?
    core_parser parser_; //!< core parser
  };
}; // namespace dbgen3
#endif
