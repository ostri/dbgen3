#ifndef ODBC_DB_HPP
#define ODBC_DB_HPP
#include <array>
#include <sqlcli1.h>
namespace db
{
  enum class BUF_TYPE
  {
    par = 0, //!< parameter
    res = 1, //!< result
    // unk = 2, //!< unknown / uninitialized
  };

  enum class ATTR_TYPE
  {
    unknown = 0,
    atomic  = 1,
    string  = 2,
    bstring = 3,
  };
  constexpr int       c_type(int db_type);
  constexpr ATTR_TYPE attr_type(int db_type);
  consteval ATTR_TYPE attr_type_c(int db_type);
  // TODO(ostri): https://github.com/serge-sans-paille/frozen
  namespace // NOLINT clang-tidy(cert-dcl59-cpp)
  {
    class type_dscr
    {
    public:
      int         db_type_;   //!< odbc db type
      int         c_type_;    //!< c type
      ATTR_TYPE   attr_type_; //!< buffer attribute type
      const char* ct_name_;   //!< c/c++ type name
      const char* dbt_name_;  //!< database type name
    };
    using AT = ATTR_TYPE;
    // clang-format off
    // NOLINTNEXTLINE clang-tidy(misc-definitions-in-headers)
    static constexpr const std::array<type_dscr, 21> dscr_
    //constinit static const type_dscr dscr_[] //NOLINT
    {{ // DB type          c/c++ type            attr type   ct name                  db type name
      {SQL_UNKNOWN_TYPE,   SQL_UNKNOWN_TYPE,     AT::unknown, "unknown",              "unknown"},
      {SQL_BOOLEAN,        SQL_C_TINYINT,        AT::atomic,  "int",                  "SQL_BOOLEAN"}, //  8
      {SQL_TINYINT,        SQL_C_TINYINT,        AT::atomic,  "int8_t",               "SQL_TINYINT"}, //  8
      {SQL_SMALLINT,       SQL_C_SHORT,          AT::atomic,  "int16_t",              "SQL_SMALLINT"}, // 16
      {SQL_INTEGER,        SQL_C_LONG,           AT::atomic,  "int32_t",              "SQL_INTEGER"}, // 32
      {SQL_BIGINT,         SQL_C_SBIGINT,        AT::atomic,  "int64_t",              "SQL_BIGINT"}, // 64
      // floating point
      {SQL_REAL,           SQL_C_FLOAT,          AT::atomic,  "float",                "SQL_REAL"}, // real
      {SQL_FLOAT,          SQL_C_FLOAT,          AT::atomic,  "float",                "SQL_FLOAT"}, // float
      {SQL_DOUBLE,         SQL_C_DOUBLE,         AT::atomic,  "double",               "SQL_DOUBLE"}, // double
      // strings
      {SQL_CHAR,           SQL_C_CHAR,           AT::string,  "char",                 "SQL_CHAR"}, 
      {SQL_VARCHAR,        SQL_C_CHAR,           AT::string,  "char",                 "SQL_VARCHAR"}, 
      {SQL_LONGVARCHAR,    SQL_C_CHAR,           AT::string,  "char",                 "SQL_LONGVARCHAR"}, 
      // binary strings
      {SQL_BINARY,         SQL_C_CHAR,           AT::bstring,  "std::uint8_t",        "SQL_BINARY"}, 
      {SQL_VARBINARY,      SQL_C_CHAR,           AT::bstring,  "std::uint8_t",        "SQL_VARBINARY"}, 
      {SQL_LONGVARBINARY,  SQL_C_CHAR,           AT::bstring,  "std::uint8_t",        "SQL_LONGVARBINARY"}, 
      // decimal strings
      {SQL_DECIMAL,        SQL_C_CHAR,           AT::string,  "char",                 "SQL_DECIMAL"}, 
      {SQL_NUMERIC,        SQL_C_CHAR,           AT::string,  "char",                 "SQL_NUMERIC"}, 
      {SQL_DECFLOAT,       SQL_C_CHAR,           AT::string,  "char",                 "SQL_DECFLOAT"}, 
      // date & time
      {SQL_TYPE_DATE,      SQL_C_TYPE_DATE,      AT::atomic,  "SQL_DATE_STRUCT",      "SQL_TYPE_DATE"}, // binary date
      {SQL_TYPE_TIME,      SQL_C_TYPE_TIME,      AT::atomic,  "SQL_TIME_STRUCT",      "SQL_TYPE_TIME"}, // binary time
      {SQL_TYPE_TIMESTAMP, SQL_C_TYPE_TIMESTAMP, AT::atomic,  "SQL_TIMESTAMP_STRUCT", "SQL_TYPE_TIMESTAMP"}, // binary timestamp
      // TODO(ostri): timestamp with time zone
      // TODO(ostri): xLOB and XML
    }};
    // clang-format on
  }; // namespace
  constexpr int c_type(int db_type)
  {
    for (auto el : dscr_)
      if (el.db_type_ == db_type) return el.c_type_;
    return SQL_UNKNOWN_TYPE; // unknown type
  }
  constexpr const char* ct_name(int db_type)
  {
    for (auto el : dscr_)
      if (el.db_type_ == db_type) return el.ct_name_;
    return SQL_UNKNOWN_TYPE; // unknown type
  }

  constexpr const char* dbt_name(int db_type)
  {
    for (auto el : dscr_)
      if (el.db_type_ == db_type) return el.dbt_name_;
    return SQL_UNKNOWN_TYPE; // unknown type
  }
  constexpr ATTR_TYPE attr_type(int db_type)
  {
    for (auto el : dscr_)
      if (el.db_type_ == db_type) return el.attr_type_;
    return AT::unknown; // unknown type
  }
  inline constexpr bool is_something(int db_type, ATTR_TYPE attr_type)
  {
    // NOLINTNEXTLINE clang-tidy(readability-use-anyofallof)
    for (const auto& cnt : dscr_)
      if ((cnt.db_type_ == db_type) && (cnt.attr_type_ == attr_type)) return true;
    return false;
  }
  constexpr bool is_atomic(int db_type) { return is_something(db_type, ATTR_TYPE::atomic); }
  constexpr bool is_string(int db_type) { return is_something(db_type, ATTR_TYPE::string); }
  constexpr bool is_bstring(int db_type) { return is_something(db_type, ATTR_TYPE::bstring); }
} // namespace db
#endif // ODBC_DB_HPP
