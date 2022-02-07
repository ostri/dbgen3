/*!
 * \file
 * \brief program to test runtime library
 */
#include <string_view>
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

using cstr_t = std::string_view;

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
  UT::Q_1::qp_id<2> qp;
  qp.set_par_1(10);            // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
  qp.set_par_1(11, 1);         // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
  REQUIRE_EQ(qp.par_1(), 10);  // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
  REQUIRE_EQ(qp.par_1(1), 11); // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)

  UT::Q_1::qr_id<2> qr;
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
  const auto* str1 = "fix string";
  const auto* str2 = "var str";
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
/*...........................................................................*/
static int diag(int rc, const db::statement& a_stmt, cstr_t a_msg = "")
{
  if (rc != SQL_SUCCESS)
  {
    std::cerr << db::error(a_stmt.get_stmt_handle(), SQL_HANDLE_STMT).dump(a_msg) << std::endl;
    std::cerr << "rc code:" << rc << std::endl;
  }
  return rc;
}
/*...........................................................................*/
static int diag_with_W(int rc, const db::statement& a_stmt, cstr_t a_msg = "")
{
  if ((rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO))
  {
    std::cerr << db::error(a_stmt.get_stmt_handle(), SQL_HANDLE_STMT).dump(a_msg) << std::endl;
    std::cerr << "rc code:" << rc << std::endl;
  }
  return rc;
}

/*...........................................................................*/
TEST_CASE("full_cycle") // NOLINT
{
  db::connection c("test");

  { // cleanup
    UT::del_tbl_rec::utl q(&c);
    auto                       rc = q.exec();
    bool res = (rc == SQL_SUCCESS) || (rc == SQL_SUCCESS_WITH_INFO) || (rc == SQL_NO_DATA_FOUND);
    if (! res) diag_with_W(rc, q, "empty table: ");
    REQUIRE(res == true);
    c.commit();
  }
  {
    // insert new record(s)
    auto        rc  = SQL_SUCCESS;
    const auto* sql = "insert into tbl (C1_smallint, C2_int, C3_bigint) values (?,?,?)";
    db::atomic<int16_t, 5, 0, 3> val1; // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
    db::atomic<int32_t, 4, 0, 3> val2;
    // NOLINTNEXTLINE clang-tidy(cppcoreguidelines-avoid-magic-numbers)
    db::atomic<int64_t, -5, 0, 3> val3;
    db::statement                 s(&c, sql); //!< final cleanup

    for (auto cnt = 0UL; cnt < val1.size(); cnt++)
    {
      // NOLINTNEXTLINE clang-tidy(cppcoreguidelines-avoid-magic-numbers)
      val1.set_value(static_cast<int16_t>((cnt + 1) * 10 + 1),
                     cnt); //
                           // NOLINTNEXTLINE clang-tidy(cppcoreguidelines-avoid-magic-numbers)
      val2.set_value(static_cast<int32_t>((cnt + 1) * 10 + 2),
                     cnt); //
                           // NOLINTNEXTLINE clang-tidy(cppcoreguidelines-avoid-magic-numbers)
      val3.set_value(static_cast<int64_t>((cnt + 1) * 10 + 3), cnt); //
    }

    rc = s.set_attr(SQL_ATTR_PARAMSET_SIZE, static_cast<int>(val1.size()));
    diag(rc, s, "set attr n records");

    rc = s.prepare();
    diag(rc, s, "prepare");

    auto h = s.get_stmt_handle();
    rc     = val1.bind_par(h, 1);
    diag(rc, s, "bind par smallint");
    rc = val2.bind_par(h, 2);
    diag(rc, s, "bind par int ");
    rc = val3.bind_par(h, 3);
    diag(rc, s, "bind par bigint");

    rc = s.exec();
    diag(rc, s, "exec");
    REQUIRE(rc == SQL_SUCCESS);
    s.commit();
  }
}