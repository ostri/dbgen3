/*!
 * \file
 * \brief program to test runtime library
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <cstdlib>
#include <iostream>

#include "connection.hpp"
#include "error_exception.hpp"
#include "program_status.hpp"

/**
 * @brief basic connection test
 *
 * @param argc nuumber of command line parameters
 * @param argv value of the command line parameters
 * @return
 */
int do_main(int /*argc*/, char** /*argv*/)
{
  try
    {
      auto show = std::getenv("TRACE") != 0; // NOLINT -concurrency-mt-unsafe,readability-implicit-bool-conversion
      db::connection conn("test1", "", "", &db::_log_, show);
    }
  catch (db::error_exception& e)
    {
      std::cerr << "\ncli error\n" << e.what() << std::endl;
      return static_cast<int>(dbgen3::p_sts::unk_db_name);
    }
  catch (...)
    {
      std::cerr << "\nunhandled exception\n" << std::endl;
    }
  return 0;
}

TEST_CASE("Test function")
{ //
  char* par_v[] =  {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
  REQUIRE(do_main(1, par_v) == 0);
}
