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
#include "exceptions.hpp"

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
    int parse_set(const str_vec& gsql_files)
    {
      auto sts = 0;
      for (const auto& file: gsql_files) 
      {
        parse_file(file);
      }
      return sts;
    }
    int parse_file(const std::string& a_filename)
    {
      auto sts=0;
      if (file_exists(a_filename))
      {
        parser_.parse_file(a_filename);
      }
      else err << out::sl("Filename: "+a_filename+" does not exists.") ;
      
      //throw gsql_file_not_exsts(a_filename);
      return sts;
    }
  private:
    bool        valid_;  //!< is instance valid ?
    core_parser parser_; //!< core parser
  };
}; // namespace dbgen3
#endif
