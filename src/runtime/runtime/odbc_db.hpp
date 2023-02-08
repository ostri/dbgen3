#ifndef ODBC_DB_HPP
#define ODBC_DB_HPP
#include <array>
#include <sqlcli1.h>
#include <string_view>
namespace db
{
  using cstr_t = std::string_view;
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
  constexpr ATTR_TYPE attr_type_c(int db_type);
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
    static constexpr const std::array<type_dscr, 23> dscr_
    //constinit static const type_dscr dscr_[] //NOLINT
    {{ // DB type          c/c++ type            attr type   ct name                  db type name
      {SQL_UNKNOWN_TYPE,   SQL_UNKNOWN_TYPE,     AT::unknown, "unknown",          "unknown"},
      {SQL_BOOLEAN,        SQL_C_TINYINT,        AT::atomic,  "int",              "SQL_BOOLEAN"}, //  8
      {SQL_TINYINT,        SQL_C_TINYINT,        AT::atomic,  "int8_t",           "SQL_TINYINT"}, //  8
      {SQL_SMALLINT,       SQL_C_SHORT,          AT::atomic,  "int16_t",          "SQL_SMALLINT"}, // 16
      {SQL_INTEGER,        SQL_C_LONG,           AT::atomic,  "int32_t",          "SQL_INTEGER"}, // 32
      {SQL_BIGINT,         SQL_C_SBIGINT,        AT::atomic,  "int64_t",          "SQL_BIGINT"}, // 64
      // floating point
      {SQL_REAL,           SQL_C_FLOAT,          AT::atomic,  "float",            "SQL_REAL"}, // real
      {SQL_FLOAT,          SQL_C_FLOAT,          AT::atomic,  "float",            "SQL_FLOAT"}, // float
      {SQL_DOUBLE,         SQL_C_DOUBLE,         AT::atomic,  "double",           "SQL_DOUBLE"}, // double
      // strings
      {SQL_CHAR,           SQL_C_CHAR,           AT::string,  "cstr_t",           "SQL_CHAR"},
      {SQL_VARCHAR,        SQL_C_CHAR,           AT::string,  "cstr_t",           "SQL_VARCHAR"},
      {SQL_LONGVARCHAR,    SQL_C_CHAR,           AT::string,  "cstr_t",           "SQL_LONGVARCHAR"},
      // binary strings
      {SQL_BINARY,         SQL_C_BINARY,         AT::bstring, "cbstr_t",          "SQL_BINARY"},
      {SQL_VARBINARY,      SQL_C_BINARY,         AT::bstring, "cbstr_t",          "SQL_VARBINARY"},
      {SQL_LONGVARBINARY,  SQL_C_BINARY,         AT::bstring, "cbstr_t",          "SQL_LONGVARBINARY"},
      // decimal strings
      {SQL_DECIMAL,        SQL_C_CHAR,           AT::string,  "cstr_t",           "SQL_DECIMAL"},
      {SQL_NUMERIC,        SQL_C_CHAR,           AT::string,  "cstr_t",           "SQL_NUMERIC"},
      {SQL_DECFLOAT,       SQL_C_CHAR,           AT::string,  "cstr_t",           "SQL_DECFLOAT"},
      // date & time
      {SQL_TYPE_DATE,      SQL_C_TYPE_DATE,      AT::atomic,  "DATE_STRUCT",      "SQL_TYPE_DATE"}, // binary date
      {SQL_TYPE_TIME,      SQL_C_TYPE_TIME,      AT::atomic,  "TIME_STRUCT",      "SQL_TYPE_TIME"}, // binary time
      {SQL_TYPE_TIMESTAMP, SQL_C_TYPE_TIMESTAMP, AT::atomic,  "TIMESTAMP_STRUCT", "SQL_TYPE_TIMESTAMP"}, // binary timestamp
      // TODO(ostri): timestamp with time zone
      {SQL_BLOB,           SQL_C_BINARY,         AT::bstring, "cbstr_t",          "SQL_BLOB"}, // long binary string
      {SQL_CLOB,           SQL_C_CHAR,           AT::string,  "cstr_t",           "SQL_CLOB"}, // long string
      // TODO(ostri): DBLOB and XML
    }};
    // clang-format on
  }; // namespace
  constexpr int c_type(int db_type)
  {
    for (auto el = 0UL; el < dscr_.size(); el++) // NOLINT modernize-loop-convert
      if (dscr_.at(el).db_type_ == db_type) return dscr_.at(el).c_type_;
    return SQL_UNKNOWN_TYPE; // unknown type
  }
  constexpr const char* ct_name(int db_type)
  {
    for (auto el = 0UL; el < dscr_.size(); el++) // NOLINT modernize-loop-convert
      if (dscr_.at(el).db_type_ == db_type) return dscr_.at(el).ct_name_;
    return SQL_UNKNOWN_TYPE; // unknown type
  }

  constexpr const char* dbt_name(int db_type)
  {
    for (auto el = 0UL; el < dscr_.size(); el++) // NOLINT modernize-loop-convert
      if (dscr_.at(el).db_type_ == db_type) return dscr_.at(el).dbt_name_;
    return SQL_UNKNOWN_TYPE; // unknown type
  }
  constexpr ATTR_TYPE attr_type(int db_type)
  {
    for (auto el = 0UL; el < dscr_.size(); el++) // NOLINT modernize-loop-convert
      if (dscr_.at(el).db_type_ == db_type) return dscr_.at(el).attr_type_;
    return AT::unknown; // unknown type
  }
  inline constexpr bool is_something(int db_type, ATTR_TYPE attr_type)
  {
    // NOLINTNEXTLINE clang-tidy(readability-use-anyofallof)
    for (auto el = 0UL; el < dscr_.size(); el++) // NOLINT modernize-loop-convert
      if ((dscr_.at(el).db_type_ == db_type) && (dscr_.at(el).attr_type_ == attr_type)) return true;
    return false;
  }
  constexpr bool is_atomic(int db_type) { return is_something(db_type, ATTR_TYPE::atomic); }
  constexpr bool is_string(int db_type) { return is_something(db_type, ATTR_TYPE::string); }
  constexpr bool is_bstring(int db_type) { return is_something(db_type, ATTR_TYPE::bstring); }

  constexpr std::pair<cstr_t, cstr_t> db_types(uint ndx)
  {
    if (ndx < dscr_.size()) return std::make_pair(dscr_.at(ndx).dbt_name_, dscr_.at(ndx).ct_name_);
    return {};
  }
  constexpr std::size_t size() { return dscr_.size(); }
} // namespace db
#endif // ODBC_DB_HPP
