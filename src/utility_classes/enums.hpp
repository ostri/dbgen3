#ifndef ENUMS_HPP
#define ENUMS_HPP

#undef MAGIC_ENUM_RANGE_MIN
#undef MAGIC_ENUM_RANGE_MAX
#define MAGIC_ENUM_RANGE_MIN -1000
#define MAGIC_ENUM_RANGE_MAX 1000
#include <magic_enum.hpp>

#include "common.hpp"

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

  enum class NT : int // xercesc nodetypes
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

  enum class DBN : int16_t
  {
    NO_NULLS         = 0, //<! no null
    NULLABLE         = 1, //<! NULL
    NULLABLE_UNKNOWN = 2  //!< we dont know (default null)
  };

  enum class VERBOSE
  {
    FALSE = 0, //<! non verbose execution
    TRUE  = 1, //<! verbose execution
  };

  enum class ODBC_TYPE : int
  {
    /* Standard SQL data types */
    UNKNOWN_TYPE                 = 0,
    CHAR                         = 1,
    NUMERIC                      = 2,
    DECIMAL                      = 3,
    INTEGER                      = 4,
    SMALLINT                     = 5,
    FLOAT                        = 6,
    REAL                         = 7,
    DOUBLE                       = 8,
    DATETIME                     = 9,
    INTERVAL                     = 10,
    TIMESTAMP                    = 11,
    VARCHAR                      = 12,
    BOOLEAN                      = 16,
    ROW                          = 19,
    TYPE_DATE                    = 91,
    TYPE_TIME                    = 92,
    TYPE_TIMESTAMP               = 93,
    TYPE_TIMESTAMP_WITH_TIMEZONE = 95,
    LONGVARCHAR                  = -1,
    BINARY                       = -2,
    VARBINARY                    = -3,
    LONGVARBINARY                = -4,
    BIGINT                       = -5,
    TINYINT                      = -6,
    BIT                          = -7,
    WCHAR                        = -8,
    WVARCHAR                     = -9,
    WLONGVARCHAR                 = -10,
    GUID                         = -11,
    GRAPHIC                      = -95,
    VARGRAPHIC                   = -96,
    LONGVARGRAPHIC               = -97,
    BLOB                         = -98,
    CLOB                         = -99,
    DBCLOB                       = -350,
    DECFLOAT                     = -360,
    XML                          = -370,
    CURSORHANDLE                 = -380,
    DATALINK                     = -400,
    USER_DEFINED_TYPE            = -450
  };

  enum class FILE_TYPE : int
  {
    hpp = 1, //!< it denotes hpp header file
    cpp = 2, //!< it denotes cpp source file
  };
}; // namespace dbgen3

#endif // ENUMS_HPP
