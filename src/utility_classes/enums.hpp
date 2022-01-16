#ifndef ENUMS_HPP
#define ENUMS_HPP

#include "common.hpp"
#include "magic_enum.hpp"

namespace dbgen3
{
  namespace ME = magic_enum;

  enum class PHASE
  {
    invalid = 0, //!< placeholder for all invalid inputs
    prepare = 1, //!< SQL to prepare environment for generator
    main    = 2, //!< SQL that is actually executed at runtime
    cleanup = 3, //!< SQL to teardown the environment for generator
  };

  enum class RDBMS
  {
    sql     = 0, //!< standard / generic sql
    db2     = 1, //!< IBM db2
    oracle  = 2, //!< Oracle database - placeholder
    mssql   = 3, //!< Microsoft SQL server - placeholder
    mariadb = 4  //!< Maria database (MYSQL clone) - placeholder
  };

  enum class NT // xercesc nodetypes
  {
    ELEMENT_NODE                = 1,
    ATTRIBUTE_NODE              = 2,
    TEXT_NODE                   = 3,
    CDATA_SECTION_NODE          = 4,
    ENTITY_REFERENCE_NODE       = 5,
    ENTITY_NODE                 = 6,
    PROCESSING_INSTRUCTION_NODE = 7,
    COMMENT_NODE                = 8,
    DOCUMENT_NODE               = 9,
    DOCUMENT_TYPE_NODE          = 10,
    DOCUMENT_FRAGMENT_NODE      = 11,
    NOTATION_NODE               = 12
  };

  enum class BUF_TYPE
  {
    par = 0, //!< parameter
    res = 1, //!< result
    unk = 2, //!< unknown / uninitialized
  };
  /// list of supported language generators
  enum class PROG_LANG
  {
    invalid = 0, //!< placeholder for all invalid inputs
    cpp     = 1, //!< c++ code generation
    java    = 2  //!< java code generation
  };

  enum class VERBOSE
  {
    FALSE = 0, //<! non verbose execution
    TRUE  = 1, //<! verbose execution
  };

  class enums
  {
  public:
    static str_vec lang_str() { return lang_str_; }
  private:
    static str_vec lang_str_;
  };
}; // namespace dbgen3

#endif // ENUMS_HPP
