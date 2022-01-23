/*!
 * \file
 * \brief program to test template header
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <iostream>
#include "dbgen3_templ.hpp"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <sqlcli.h>
#include "dbgen3_templ.hpp"

TEST_SUITE("templates")
{
  TEST_CASE("atomic")
  {
    const auto           v   = 10;
    constexpr auto       dim = 5;
    db::atomic<int, dim> f1;
    REQUIRE_EQ(f1.size(), dim);
    f1.set_value(v);
    REQUIRE_EQ(f1.value(), v);
    REQUIRE_EQ(f1.value(), f1.value(0));
    f1.set_value(v, dim - 1);
    REQUIRE_EQ(f1.value(dim - 1), v);
    CHECK_THROWS_AS(f1.set_value(v, dim), std::out_of_range);
    CHECK_THROWS_AS(f1.value(dim), std::out_of_range);

    db::atomic<int, 0> f0;
    REQUIRE_EQ(f0.size(), 0);
    //  fails with SIGSEGV
    //    CHECK_THROWS_AS(f0.set_value(v, 0), std::out_of_range);
    //    CHECK_THROWS_AS(f0.value(), std::out_of_range);
  }
  TEST_CASE("string")
  {
    constexpr auto           dim     = 5;
    constexpr auto           v       = "0123456789";
    constexpr auto           v0      = "";
    auto                     v1      = std::string(v) + "A";
    constexpr auto           str_len = 10; // must be v.size()
    db::string<str_len, dim> f1;
    REQUIRE_EQ(f1.size(), dim);
    f1.set_value(v);
    REQUIRE_EQ(f1.value(), v);
    REQUIRE_EQ(f1.value(), f1.value(0));
    f1.set_value(v, dim - 1);
    REQUIRE_EQ(f1.value(dim - 1), v);
    CHECK_THROWS_AS(f1.set_value(v, dim), std::out_of_range);
    CHECK_THROWS_AS(f1.value(dim), std::out_of_range);
    f1.set_value(v0);
    REQUIRE_EQ(f1.value(), v0);
    f1.set_value(v1);
    REQUIRE_EQ(f1.value(), v); // yes v and not v1

    db::string<str_len, 0> f0;
    REQUIRE_EQ(f0.size(), 0);
  }
  TEST_CASE("mem_blk")
  {
    using Q = std::array<int, 5>;

    constexpr auto dim = 3;
    constexpr auto v   = Q{1, 2, 3, 4, 5};
    constexpr auto v0  = Q{};
    auto           v1  = std::array{1, 2, 3, 4, 5, 6};

    db::mem_blk<5, dim, int> f1;
    REQUIRE_EQ(f1.size(), dim);
    f1.set_value(v);
    REQUIRE_EQ(std::equal(f1.value().begin(), f1.value().end(), v.begin()), true);
    REQUIRE_EQ(std::equal(f1.value().begin(), f1.value().end(), f1.value(0).begin()), true);
    f1.set_value(v, dim - 1);
    REQUIRE_EQ(std::equal(f1.value(dim - 1).begin(), f1.value(dim - 1).end(), v.begin()), true);
    CHECK_THROWS_AS(f1.set_value(v, dim), std::out_of_range);
    CHECK_THROWS_AS(f1.value(dim), std::out_of_range);
    f1.set_value(v0);
    REQUIRE_EQ(std::equal(f1.value().begin(),f1.value().end(), v0.begin()), true);
    f1.set_value(std::span(v1.data(), v1.size()));
    REQUIRE_EQ(std::equal(f1.value().begin(), f1.value().end(), v.begin()), true); // yes v and not v1

    db::mem_blk<5, 0, char> f0;
    REQUIRE_EQ(f0.size(), 0);
  }
}
