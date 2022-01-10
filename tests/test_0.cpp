/*!
 * \file
 * \brief program to test runtime library
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <cstdlib>
#include <iostream>
#include <array>

#include "connection.hpp"
#include "error_exception.hpp"
#include "program_status.hpp"

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
    return static_cast<int>(dbgen3::p_sts::success);
  }
  catch (db::error_exception& e)
  {
    db::_log_("cli error");
    db::_log_(e.what());
    return static_cast<int>(dbgen3::p_sts::unk_db_name);
  }
  catch (...)
  {
    db::_log_("unhandled exception");
    return static_cast<int>(dbgen3::p_sts::unk_exception);
  }
}

TEST_CASE("Connection constructor")
{
  REQUIRE(do_main(argv_t{"", "test"}) == static_cast<int>(dbgen3::p_sts::success));
  REQUIRE(do_main(argv_t{"", "unknown"}) == static_cast<int>(dbgen3::p_sts::unk_db_name));
  REQUIRE(do_main(argv_t{"", ""}) == static_cast<int>(dbgen3::p_sts::unk_db_name));
  REQUIRE(do_main(argv_t{"", nullptr}) == static_cast<int>(dbgen3::p_sts::unk_exception));
}
