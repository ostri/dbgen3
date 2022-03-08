/*!
 * \file
 * \brief program to test template header
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <iostream>
#include <runtime/connection.hpp>
#include <runtime/statement.hpp>
#include <sqlcli.h>

#include "runtime/dbgen3_templ.hpp"
//#include "enums.hpp"


using cstr_t = std::string_view;

static bool cmp(const db::cbstr_t& v1, const db::cbstr_t& v2)
{
  if (v1.size() != v2.size()) return false;
  auto len = std::min(v1.size(), v2.size());
  return std::memcmp(v1.data(), v2.data(), len) == 0;
}

TEST_SUITE("templates")
{
  TEST_CASE("atomic") // NOLINT
  {
    const auto                            v        = 10;
    constexpr auto                        dim      = 5;
    constexpr auto                        smallint = 5;
    db::atomic<int16_t, smallint, dim, 0> f1;
    REQUIRE_EQ(f1.size(), dim);
    f1.set_value(v);
    REQUIRE_EQ(f1.value(), v);
    REQUIRE_EQ(f1.value(), f1.value(0));
    f1.set_value(v, dim - 1);
    REQUIRE_EQ(f1.value(dim - 1), v);
    // NOLINTNEXTLINE check-tidy(llvm-include-order)
    CHECK_THROWS_AS(f1.set_value(v, dim), std::out_of_range);
    // NOLINTNEXTLINE check-tidy(llvm-include-order)
    CHECK_THROWS_AS(f1.value(dim), std::out_of_range);

    // vector with dimension 0
    db::atomic<int16_t, smallint, 0, 0> f0;
    REQUIRE_EQ(f0.size(), 0);
    //  fails with SIGSEGV
    //    CHECK_THROWS_AS(f0.set_value(v, 0), std::out_of_range);
    //    CHECK_THROWS_AS(f0.value(), std::out_of_range);
  }
  TEST_CASE("string") // NOLINT
  {
    constexpr std::size_t    dim     = 5UL;
    constexpr cstr_t         v       = "0123456789";
    constexpr auto           v0      = "";
    auto                     v1      = std::string(v) + "A";
    constexpr auto           str_len = v.size(); // must be v.size()
    db::string<std::array<char, str_len>, SQL_CHAR,  dim, 0, char> f1;
    REQUIRE(f1.size() == dim);
    f1.set_value(v);
    REQUIRE_EQ(f1.value(), v);
    REQUIRE_EQ(f1.value(), f1.value(0));
    f1.set_value(v, dim - 1);
    REQUIRE_EQ(f1.value(dim - 1), v);
    // NOLINTNEXTLINE check-tidy(llvm-include-order)
    CHECK_THROWS_AS(f1.set_value(v, dim), std::out_of_range);
    // NOLINTNEXTLINE check-tidy(llvm-include-order)
    CHECK_THROWS_AS(f1.value(dim), std::out_of_range);
    f1.set_value(v0);
    REQUIRE_EQ(f1.value(), v0);
    f1.set_value(v1);
    REQUIRE_EQ(f1.value(), v); // yes v and not v1

    db::string<std::array<char, str_len>, SQL_CHAR, 0, 0, char> f0;
    REQUIRE_EQ(f0.size(), 0);
  }
  TEST_CASE("bstring") // NOLINT
  {
    using Q = std::array<uint8_t, 5>; // NOLINT clang-tidy(cppcoreguidelines-avoid-magic-numbers)

    constexpr auto dim = 3UL;
    constexpr auto v   = Q{1, 2, 3, 4, 5};
    constexpr auto v0  = Q{};
    // NOLINTNEXTLINE clang-tidy(cppcoreguidelines-avoid-magic-numbers)
    auto      v1 = std::array<uint8_t, 6>{1, 2, 3, 4, 5, 6};
    std::span s{v};

    db::bstring<std::array<uint8_t,5>, SQL_BINARY, dim, uint8_t> f1;
    REQUIRE(f1.size() == dim);
    f1.set_value(s);
    REQUIRE_EQ(cmp(f1.value(),std::span(v)), true);
    REQUIRE_EQ(cmp(f1.value(),f1.value(0)),  true); ///
    f1.set_value(v, dim - 1);
    REQUIRE_EQ(std::equal(f1.value(dim - 1).begin(), f1.value(dim - 1).end(), v.begin()), true);
    // NOLINTNEXTLINE check-tidy(llvm-include-order)
    CHECK_THROWS_AS(f1.set_value(v, dim), std::out_of_range);
    // NOLINTNEXTLINE check-tidy(llvm-include-order)
    CHECK_THROWS_AS(f1.value(dim), std::out_of_range);
    f1.set_value(v0);
    REQUIRE_EQ(std::equal(f1.value().begin(), f1.value().end(), v0.begin()), true);
    f1.set_value(std::span<uint8_t>(v1.data(), v1.size()));
    REQUIRE_EQ(std::equal(f1.value().begin(), f1.value().end(), v.begin()),
               true); // yes v and not v1

    db::bstring<std::array<char, 5>, SQL_BINARY, 0, char> f0;
    REQUIRE_EQ(f0.size(), 0);
  }

  static int diag(int rc, const db::statement& a_stmt, cstr_t a_msg = "")
  {
    if (rc != SQL_SUCCESS)
      std::cerr << db::error(a_stmt.handle(), SQL_HANDLE_STMT).dump(a_msg) << std::endl;
    return rc;
  }
  TEST_CASE("full_cycle") // NOLINT
  {
    db::connection c("test");


    { // cleanup
      const auto*   sql_2 = "delete from tbl where C1_smallint > 0";
      db::statement s(&c, sql_2); //!< final cleanup
      s.exec_direct(sql_2, false);
      s.commit();
    }

    // {                            // populate
    //    auto           sql_1 = "insert into tbl (C1_smallint) values (1),(2),(3)";
    //   db::statement s(c, sql_1); //!< ok
    //   auto          rc = s.exec_direct(sql_1, false);
    //   diag(rc, s, "populate");
    //   s.commit();
    //   REQUIRE(rc == SQL_SUCCESS);
    // }
    {
      // insert new record
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

      auto h = s.handle();
      rc     = val1.bind_par(h, 1);
      diag(rc, s, "bind par smallint");
      rc = val2.bind_par(h, 2);
      diag(rc, s, "bind par int ");
      rc = val3.bind_par(h, 3);
      diag(rc, s, "bind par bigint");

      rc = s.exec();
      diag(rc, s, "exec");
      REQUIRE(rc == SQL_SUCCESS);
    }
    {
      auto        rc  = SQL_SUCCESS;
      const auto* sql = "select C1_smallint, C2_int, C3_bigint from tbl";

      // NOLINTNEXTLINE clang-tidy(cppcoreguidelines-avoid-magic-numbers)
      db::atomic<int16_t, 5, 0, 2> val1;
      db::atomic<int32_t, 4, 0, 2> val2;
      // NOLINTNEXTLINE clang-tidy(cppcoreguidelines-avoid-magic-numbers)
      db::atomic<int64_t, -5, 0, 2> val3;
      db::statement                 s(&c, sql); //!< final cleanup

      rc = s.prepare();
      diag(rc, s, "prepare");
      REQUIRE(rc == SQL_SUCCESS);

      rc = s.set_attr_l(SQL_ATTR_ROW_ARRAY_SIZE, val1.size());
      diag(rc, s, "set attr n records");
      REQUIRE(rc == SQL_SUCCESS);

      std::size_t rows_read;
      rc = s.set_attr(SQL_ATTR_ROWS_FETCHED_PTR, &rows_read);
      diag(rc, s, "set results variable");
      REQUIRE(rc == SQL_SUCCESS);

      rc = val1.bind_col(s.handle(), 1);
      diag(rc, s, "bind col smallint");
      REQUIRE(rc == SQL_SUCCESS);
      rc = val2.bind_col(s.handle(), 2);
      diag(rc, s, "bind col int");
      REQUIRE(rc == SQL_SUCCESS);
      rc = val3.bind_col(s.handle(), 3);
      diag(rc, s, "bind col bigint");
      REQUIRE(rc == SQL_SUCCESS);

      rc = s.exec();
      diag(rc, s, "exec");
      REQUIRE(rc == SQL_SUCCESS);

      while (rc == SQL_SUCCESS)
      {
        rc = s.fetch_scroll(SQL_FETCH_NEXT, val1.size(), false);
        std::cerr << val1.dump_all("final result 16:", rows_read, 0) << std::endl;
        std::cerr << val2.dump_all("final result 32:", rows_read, 0) << std::endl;
        std::cerr << val3.dump_all("final result 64:", rows_read, 0) << std::endl;
        if ((rc != SQL_SUCCESS) && (rc != SQL_NO_DATA)) diag(rc, s, "fetch");
        REQUIRE(((rc == SQL_SUCCESS) || (rc == SQL_NO_DATA)));
        if (rows_read < val1.size()) break;
      }
      s.commit();
    }
  }
}
