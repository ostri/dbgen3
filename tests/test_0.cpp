/*!
 * \file
 * \brief program to test runtime library
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <array>
#include <cstdlib>
#include <doctest/doctest.h>
#include <iostream>

#include "connection.hpp"
#include "error_exception.hpp"
#include "program_status.hpp"
#include "statement.hpp"

using argv_t = std::vector<const char*>;
/**
 * @brief basic connection test
 *
 * @param argc nuumber of command line parameters
 * @param argv value of the command line parameters
 * @return
 */
int do_main(argv_t argv)
{
  try
  {
    db::connection conn(argv[1]);
    return static_cast<int>(dbgen3::P_STS::success);
  }
  catch (db::error_exception& e)
  {
    db::_log_("cli error->");
    db::_log_(e.what());
    return static_cast<int>(dbgen3::P_STS::unk_db_name);
  }
  catch (...)
  {
    db::_log_("unhandled exception->");
    return static_cast<int>(dbgen3::P_STS::unk_exception);
  }
}

TEST_CASE("Connection constructor") // NOLINT
{
  INFO("normal connect");
  REQUIRE(do_main(argv_t{"", "test"}) == static_cast<int>(dbgen3::P_STS::success));
  INFO("unknown database");
  REQUIRE(do_main(argv_t{"", "unknown"}) == static_cast<int>(dbgen3::P_STS::unk_db_name));
  INFO("empty database");
  REQUIRE(do_main(argv_t{"", ""}) == static_cast<int>(dbgen3::P_STS::unk_db_name));
  INFO("invalid database name");
  REQUIRE(do_main(argv_t{"", nullptr}) == static_cast<int>(dbgen3::P_STS::unk_exception));
}
TEST_CASE("statement - exec_direct") // NOLINT
{ //
  const char* sql_1 = "CREATE TABLE TBL(COL1 INT);";
  const char* sql_2 = "CREATE TABLE -- TBL(COL1 INT);";
  const char* sql_3 = "drop table tbl";

  db::connection c("test");
  db::statement  s1(&c, sql_1); //!< ok
  db::statement  s2(&c, sql_2); //!< invalid sql
  db::statement  s3(&c, sql_1); //!< logically invalid sql
  db::statement  s4(&c, sql_3); //!< table cleanup

  s4.exec_direct(sql_3, false); //!< initial cleanup we don't care if successful
                                //!< there might be a remaining table from previous run

  { // OK
    auto rc = s1.exec_direct(sql_1, false);
    REQUIRE(rc == SQL_SUCCESS);
    s1.commit();
  }
  { // invalid SQL
    auto rc = s1.exec_direct(sql_2, false);
    REQUIRE(rc == SQL_ERROR);
    s2.commit();
  }
  { // logically invalid SQL (duplicate table tbl)
    auto rc = s1.exec_direct(sql_1, false);
    REQUIRE(rc == SQL_ERROR);
    s3.commit();
  }
  { // final cleanup
    auto rc = s4.exec_direct(sql_3, false);
    REQUIRE(rc == SQL_SUCCESS);
    s4.commit();
  }
}
