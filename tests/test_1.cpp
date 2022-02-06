/*!
 * \file
 * \brief program to test runtime library
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <array>
#include <cstdlib>
#include <doctest/doctest.h>
#include <iostream>
#include <span>

#include "connection.hpp"
#include "error_exception.hpp"
#include "program_status.hpp"
#include "statement.hpp"
#include "t1.hpp"

// using argv_t = std::vector<const char*>;
// /**
//  * @brief basic connection test
//  *
//  * @param argc nuumber of command line parameters
//  * @param argv value of the command line parameters
//  * @return
//  */
// int do_main(argv_t argv)
// {
//   try
//   {
//     db::connection conn(argv[1]);
//     return static_cast<int>(dbgen3::P_STS::success);
//   }
//   catch (db::error_exception& e)
//   {
//     db::_log_("cli error->");
//     db::_log_(e.what());
//     return static_cast<int>(dbgen3::P_STS::unk_db_name);
//   }
//   catch (...)
//   {
//     db::_log_("unhandled exception->");
//     return static_cast<int>(dbgen3::P_STS::unk_exception);
//   }
// }

// TEST_CASE("Connection constructor")
// {
//   INFO("normal connect");
//   REQUIRE(do_main(argv_t{"", "test"}) == static_cast<int>(dbgen3::P_STS::success));
//   INFO("unknown database");
//   REQUIRE(do_main(argv_t{"", "unknown"}) == static_cast<int>(dbgen3::P_STS::unk_db_name));
//   INFO("empty database");
//   REQUIRE(do_main(argv_t{"", ""}) == static_cast<int>(dbgen3::P_STS::unk_db_name));
//   INFO("invalid database name");
//   REQUIRE(do_main(argv_t{"", nullptr}) == static_cast<int>(dbgen3::P_STS::unk_exception));
// }
// TEST_CASE("statement - exec_direct")
// { //
//   auto sql_1 = "CREATE TABLE TBL(COL1 INT);";
//   auto sql_2 = "CREATE TABLE -- TBL(COL1 INT);";
//   auto sql_3 = "drop table tbl";

//   db::connection c("test");
//   db::statement  s1(c, sql_1); //!< ok
//   db::statement  s2(c, sql_2); //!< invalid sql
//   db::statement  s3(c, sql_1); //!< logically invalid sql
//   db::statement  s4(c, sql_3); //!< table cleanup

//   s4.exec_direct(sql_3, false); //!< initial cleanup we don't care if successful
//                                 //!< there might be a remaining table from previous run

//   { // OK
//     auto rc = s1.exec_direct(sql_1, false);
//     REQUIRE(rc == SQL_SUCCESS);
//     s1.commit();
//   }
//   { // invalid SQL
//     auto rc = s1.exec_direct(sql_2, false);
//     REQUIRE(rc == SQL_ERROR);
//     s2.commit();
//   }
//   { // logically invalid SQL (duplicate table tbl)
//     auto rc = s1.exec_direct(sql_1, false);
//     REQUIRE(rc == SQL_ERROR);
//     s3.commit();
//   }
//   { // final cleanup
//     auto rc = s4.exec_direct(sql_3, false);
//     REQUIRE(rc == SQL_SUCCESS);
//     s4.commit();
//   }
// }
template <typename T>
static bool cmp(const T& v1, const T& v2)
{
  return std::memcmp(&v1, &v2, sizeof(T)) == 0;
}
static bool cmp(const db::cbstr_t& v1, const db::cbstr_t& v2)
{
  if (v1.size() != v2.size()) return false;
  auto len = std::min(v1.size(), v2.size());
  return std::memcmp(v1.data(), v2.data(), len) == 0;
}


TEST_CASE("T1 - basic generator test") // NOLINT clang-tidy(cert-err58-cpp)
{
  db::connection            c("test");
  q_set_id::q_id1::qp_id<2> qp;
  qp.set_par_1(10);            // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
  qp.set_par_1(11, 1);         // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
  REQUIRE_EQ(qp.par_1(), 10);  // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
  REQUIRE_EQ(qp.par_1(1), 11); // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)

  q_set_id::q_id1::qr_id<2> qr;
  const int16_t             val16 = 10;
  const int32_t             val32 = 10;
  const int64_t             val64 = 10;
  qr.set_c1_smallint(val16);
  qr.set_c2_int(val32);
  qr.set_c3_bigint(val64);
  const char* val_dec = "-123456789012345678901234.56789";
  qr.set_c4_numeric(val_dec);
  qr.set_c5_decimal(val_dec);
  qr.set_c6_dec(val_dec);
  const auto *str1 = "fix string";
  const auto *str2 = "var str";
  qr.set_c7_char(str1);
  qr.set_c7_char(str1, 1);
  qr.set_c8_varchar(str2);
  qr.set_c8_varchar(str2, 1);
  auto date = DATE_STRUCT{2022, 1, 22}; // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
  auto time = TIME_STRUCT{23, 59, 59};  // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
  auto ts   = TIMESTAMP_STRUCT{
    2022, 1, 22, 23, 59, 59, 99999}; // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
  qr.set_c9_date(date);
  qr.set_c10_time(time);
  qr.set_c11_timestamp(ts);
  qr.set_c11_timestamp(ts, 1);
  using a_t = std::array<uint8_t, 3>;
  using a_v = std::vector<uint8_t>;
  a_t a{1, 2, 3};
  a_v b{1, 2, 3, 4, 5, 6, 7}; // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
  qr.set_c14_char_for_bit_data(a);
  qr.set_c14_char_for_bit_data(a, 1);
  const double val_double = 12345.67;
  qr.set_c15_double(val_double);
  qr.set_c16_float(val_double);
  qr.set_c17_varchar_for_bit_data(b);
  qr.set_c17_varchar_for_bit_data(b, 1);
  REQUIRE_EQ(qr.c1_smallint(), val16);
  REQUIRE_EQ(qr.c2_int(), val32);
  REQUIRE_EQ(qr.c3_bigint(), val64);
  REQUIRE_EQ(qr.c4_numeric(), val_dec);
  REQUIRE_EQ(qr.c5_decimal(), val_dec);
  REQUIRE_EQ(qr.c6_dec(), val_dec);
  REQUIRE_EQ(qr.c7_char(), str1);
  REQUIRE_EQ(qr.c7_char(1), str1);
  REQUIRE_EQ(qr.c8_varchar(), str2);
  REQUIRE_EQ(qr.c8_varchar(1), str2);
  REQUIRE(cmp<DATE_STRUCT>(qr.c9_date(), date));
  REQUIRE(cmp<TIME_STRUCT>(qr.c10_time(), time));
  REQUIRE(cmp<TIMESTAMP_STRUCT>(qr.c11_timestamp(), ts));
  REQUIRE(cmp<TIMESTAMP_STRUCT>(qr.c11_timestamp(1), ts));
  REQUIRE(cmp(qr.c14_char_for_bit_data(), a));
  REQUIRE(cmp(qr.c14_char_for_bit_data(1), a));
  REQUIRE_EQ(qr.c15_double(), val_double);
  REQUIRE_EQ(qr.c16_float(), val_double);
  REQUIRE(cmp(qr.c17_varchar_for_bit_data(), b));
  REQUIRE(cmp(qr.c17_varchar_for_bit_data(1), b));
  std::cerr << qp.dump();
  std::cerr << qr.dump();
}