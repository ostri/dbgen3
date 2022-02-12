/*!
 * \file
 * \brief program to test runtime library
 */
#include "dbgen3_templ.hpp"
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
  db::connection    c("test");
  UT::Q_1::qp_id<2> qp;
  qp.set_par_1(10);            // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
  qp.set_par_1(11, 1);         // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
  REQUIRE_EQ(qp.par_1(), 10);  // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
  REQUIRE_EQ(qp.par_1(1), 11); // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)

  UT::Q_1::qr_id<2> qr;
  const int16_t     val16 = 10;
  const int32_t     val32 = 10;
  const int64_t     val64 = 10;
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
  qr.set_c15_char_for_bit_data(a);
  qr.set_c15_char_for_bit_data(a, 1);
  const double val_double = 12345.67;
  qr.set_c16_double(val_double);
  qr.set_c17_float(val_double);
  qr.set_c18_varchar_for_bit_data(b);
  qr.set_c18_varchar_for_bit_data(b, 1);
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
  REQUIRE(cmp(qr.c15_char_for_bit_data(), a));
  REQUIRE(cmp(qr.c15_char_for_bit_data(1), a));
  REQUIRE_EQ(qr.c16_double(), val_double);
  REQUIRE_EQ(qr.c17_float(), val_double);
  REQUIRE(cmp(qr.c18_varchar_for_bit_data(), b));
  REQUIRE(cmp(qr.c18_varchar_for_bit_data(1), b));
  std::cerr << qp.dump("buffer: parameter");
  std::cerr << qr.dump("buffer: result");
}
/*...........................................................................*/
static int diag(int rc, int handle, cstr_t a_msg = "")
{
  if (rc != SQL_SUCCESS)
  {
    std::cerr << db::error(handle, SQL_HANDLE_STMT).dump(a_msg) << std::endl;
    std::cerr << "rc code:" << rc << std::endl;
  }
  return rc;
}
/*...........................................................................*/
static int diag_with_W(int rc, int handle, cstr_t a_msg = "")
{
  if ((rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO))
  {
    std::cerr << db::error(handle, SQL_HANDLE_STMT).dump(a_msg) << std::endl;
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
    auto                 rc = q.exec("");
    bool res = (rc == SQL_SUCCESS) || (rc == SQL_SUCCESS_WITH_INFO) || (rc == SQL_NO_DATA_FOUND);
    if (! res) diag_with_W(rc, q.handle(), "empty table: ");
    REQUIRE(res == true);
    c.commit();
  }
  {                           // insert
    UT::insert::qp_5<10> par; // NOLINT
    UT::insert::utl      q(&c);
    q.set_param_buf(&par);
    cstr_t dec_const = "-234567890123456789012345.67890";
    auto   bin_char =
      std::array<uint8_t, 15>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}; // NOLINT
    auto bin_varchar = std::array<uint8_t, 5>{0, 1, 2, 3, 4};                    // NOLINT

    auto date =
      DATE_STRUCT{2022, 1, 22};          // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
    auto time = TIME_STRUCT{23, 59, 59}; // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
    auto ts   = TIMESTAMP_STRUCT{
      2022, 1, 22, 23, 59, 59, 99999}; // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
    for (auto cnt = 0UL; cnt < par.size(); cnt++)
    {
      par.set_par_1(static_cast<int16_t>(cnt + 1), cnt); // NOLINT
      par.set_par_2(static_cast<int32_t>(cnt + 1), cnt); // NOLINT
      par.set_par_3(static_cast<int64_t>(cnt + 1), cnt); // NOLINT
      par.set_par_4(dec_const, cnt);
      par.set_par_5("123.45", cnt);
      par.set_par_6("123.456", cnt);
      par.set_par_7("abcdefghij", cnt);
      par.set_par_8("ABC", cnt);
      par.set_par_9(date, cnt);
      par.set_par_10(time, cnt);
      par.set_par_11(ts, cnt);
      par.set_par_12(std::array<uint8_t, 10>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, cnt); // NOLINT
      par.set_par_13("clob", cnt);
      par.set_par_14(bin_char, cnt);    // NOLINT
      par.set_par_15(123.45, cnt);      // NOLINT
      par.set_par_16(123.456, cnt);     // NOLINT
      par.set_par_17(bin_varchar, cnt); // NOLINT
    }
    par.set_occupied(par.size());
    auto rc = q.exec();
    diag(rc, q.handle(), "insert records: ");
    REQUIRE(rc == SQL_SUCCESS);
    c.commit();
    std::cerr << q.dump("***after insert***") << std::endl;
  }
  { // select
    UT::select::qr_6<3> res;
    UT::select::utl     q(&c);
    q.set_result_buf(&res);

    auto rc = q.exec();
    diag(rc, q.handle(), "exec select: ");
    REQUIRE(rc == SQL_SUCCESS);

    while (rc == SQL_SUCCESS)
    {
      rc = q.fetch_scroll(SQL_FETCH_NEXT, res.size());
      if ((rc != SQL_SUCCESS) && (rc != SQL_NO_DATA)) diag(rc, q.handle(), "fetch");
      REQUIRE(((rc == SQL_SUCCESS) || (rc == SQL_NO_DATA)));
      std::cerr << res.dump("***select fetch***") << std::endl;
      if (q.rows_read() < res.size()) break;
    }
    c.commit();
  }
}
TEST_CASE("colums_custom_names") // NOLINT
{
  const static constexpr std::array<db::attr_data, 17> arr_  
  {{
    {"name-1", 20, 10},
    {"name-2", 21, 10},
  }};
  static_assert(db::a_width(1, arr_) == 21, "zgleda da ne dela"); // NOLINT
}