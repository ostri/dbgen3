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
#include <string_view>

#include "dbgen3_templ.hpp"
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
// static int diag_with_W(int rc, int handle, cstr_t a_msg = "")
// {
//   if ((rc != SQL_SUCCESS) && (rc != SQL_SUCCESS_WITH_INFO))
//   {
//     std::cerr << db::error(handle, SQL_HANDLE_STMT).dump(a_msg) << std::endl;
//     std::cerr << "rc code:" << rc << std::endl;
//   }
//   return rc;
// }
int16_t cleanup(db::connection* c)
{
  UT::del_tbl_rec::utl q(c);
  q.exec(""); // we dont care for the status it is already empty or it is purged
  c->commit();
  return SQL_SUCCESS;
}

TEST_CASE("T1 - basic generator test") // NOLINT clang-tidy(cert-err58-cpp)
{
  db::connection c("test");
  auto           rc = cleanup(&c);
  REQUIRE_EQ(rc, SQL_SUCCESS);
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
TEST_CASE("full_cycle") // NOLINT
{
  db::connection c("test");
  REQUIRE_EQ(cleanup(&c), SQL_SUCCESS);
  {                                           // insert
    constexpr const std::size_t max_buf = 10; // 10 rows of records in inserted in one exec
    UT::insert::utl<max_buf>    q(&c);
    auto*                       par       = q.par_buf();
    cstr_t                      dec_const = "-234567890123456789012345.67890";
    auto                        bin_char =
      std::array<uint8_t, 15>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}; // NOLINT
    auto bin_varchar = std::array<uint8_t, 5>{0, 1, 2, 3, 4};                    // NOLINT

    auto date =
      DATE_STRUCT{2022, 1, 22};          // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
    auto time = TIME_STRUCT{23, 59, 59}; // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
    auto ts   = TIMESTAMP_STRUCT{
      2022, 1, 22, 23, 59, 59, 99999}; // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)
    for (auto cnt = 0UL; cnt < par->size(); cnt++)
    {
      par->set_par_1(static_cast<int16_t>(cnt + 1), cnt); // NOLINT
      par->set_par_2(static_cast<int32_t>(cnt + 1), cnt); // NOLINT
      par->set_par_3(static_cast<int64_t>(cnt + 1), cnt); // NOLINT
      par->set_par_4(dec_const, cnt);
      par->set_par_5("123.45", cnt);
      par->set_par_6("123.456", cnt);
      par->set_par_7("abcdefghij", cnt);
      par->set_par_8("ABC", cnt);
      par->set_par_9(date, cnt);
      par->set_par_10(time, cnt);
      par->set_par_11(ts, cnt);
      par->set_par_12(std::array<uint8_t, 10>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, cnt); // NOLINT
      par->set_par_13("clob", cnt);
      par->set_par_14(bin_char, cnt);    // NOLINT
      par->set_par_15(123.45, cnt);      // NOLINT
      par->set_par_16(123.456, cnt);     // NOLINT
      par->set_par_17(bin_varchar, cnt); // NOLINT
    }
    par->set_occupied(par->size());
    auto rc = q.exec();
    diag(rc, q.handle(), "insert records: ");
    REQUIRE(rc == SQL_SUCCESS);
    c.commit();
    std::cerr << q.dump("***after insert***") << std::endl;
  }
  { // select
    UT::select::utl<3> q(&c);
    auto*              res = q.res_buf();

    auto rc = q.exec();
    diag(rc, q.handle(), "exec select: ");
    REQUIRE(rc == SQL_SUCCESS);

    while (q.fetch() == SQL_SUCCESS)
    {
      std::cerr << res->dump("***select fetch***") << std::endl;
      if (q.rows_read() < res->size()) break;
    }
    c.commit();
  }
}
TEST_CASE("colums_custom_names") // NOLINT
{
  UT::named_columns::xx_p par;
  UT::named_columns::xx_r res;

  par.set_cond(1);
  res.set_char8("abc");
  res.set_dec("123");
  res.set_decimal("456");
  res.set_numeric("789");
  res.set_int16(1);
  res.set_int32(2);
  res.set_int64(3);

  REQUIRE_EQ(par.cond(), 1);
  REQUIRE_EQ(res.char8(), "abc");
  REQUIRE_EQ(res.dec(), "123");
  REQUIRE_EQ(res.decimal(), "456");
  REQUIRE_EQ(res.numeric(), "789");
  REQUIRE_EQ(res.int16(), 1);
  REQUIRE_EQ(res.int32(), 2);
  REQUIRE_EQ(res.int64(), 3);
}
TEST_CASE("null_related_tests") // NOLINT
{
  db::connection c("test");
  REQUIRE_EQ(cleanup(&c), SQL_SUCCESS);
  {
    UT::insert5::utl q(&c);
    auto*            par = q.par_buf();

    par->set_id(1);            // small int
    par->set_null_int32();     // int
    par->set_null_numeric();   // decimal / string
    par->set_null_timestamp(); // timestamp
    par->set_null_blob();      // blob

    REQUIRE(par->id() == 1);
    REQUIRE(par->is_null_int32());
    REQUIRE(par->is_null_numeric());
    REQUIRE(par->is_null_timestamp());
    REQUIRE(par->is_null_blob());

    std::cerr << q.dump("************************************");
    auto rc = q.exec();
    diag(rc, q.handle(), "insert records: ");
    REQUIRE(rc == SQL_SUCCESS);
    c.commit();
  }
  {
    UT::select::utl q(&c);
    auto*           res(q.res_buf());

    auto rc = q.exec();
    diag(rc, q.handle(), "exec select: ");
    REQUIRE(rc == SQL_SUCCESS);

    while (q.fetch() == SQL_SUCCESS)
    {
      std::cerr << res->dump("***select fetch***") << std::endl;
      if (q.rows_read() < res->size()) break;
    }
    c.commit();
    REQUIRE(res->c1_smallint() == 1);
    REQUIRE(res->is_null_c2_int());
    REQUIRE(res->is_null_c4_numeric());
    REQUIRE(res->is_null_c11_timestamp());
    REQUIRE(res->is_null_c13_blob());
  }
}
TEST_CASE("default buffer contents") // NOLINT
{
  db::connection c("test");
  REQUIRE_EQ(cleanup(&c), SQL_SUCCESS);

  {
    UT::insert::utl q(&c);

    auto* par = q.par_buf();
    REQUIRE(par->is_null_par_1());

    auto rc = q.exec();
    diag(rc, q.handle(), "insert records: ");
    REQUIRE(rc == SQL_SUCCESS);
    c.commit();
  }
}
TEST_CASE("insert and select") // NOLINT
{
  db::connection c("test");
  REQUIRE_EQ(cleanup(&c), SQL_SUCCESS);

  { // this is not an error it is how the ODBC works
    // in select (insert unless it is fom other table) you can insert and select only one record
    const uint                      par_max = 10;
    const uint                      res_max = 2;
    UT::both::utl<par_max, res_max> q(&c); // NOLINT
    auto*                           par = q.par_buf();
    auto*                           res = q.res_buf();

    for (auto cnt = 0UL; cnt < par->size(); cnt++) { par->set_par_1("xx_" + std::to_string(cnt), cnt); }
    par->set_occupied(par->size());
    std::cerr << par->dump("insert buffer") << std::endl;
    /*auto rc =*/ q.exec();
    res->set_occupied(res->size());
    while (SQL_SUCCESS == q.fetch())
    {
      std::cerr << res->dump("***select fetch***") << std::endl;
      if (q.rows_read() < res->size()) break;
    }
    c.commit();
  }
}
TEST_CASE("undefined return codes") // NOLINT clang-tidy(cert-err58-cpp)
{
  db::connection             c("test");
  UT::del_tbl_rec_throw::utl q(&c);
  REQUIRE_THROWS(q.exec()); // NOLINT
  c.rollback();
}
