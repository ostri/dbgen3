/*!
 * \file
 * \brief program to test runtime library
 */
#include <cstdlib>
#include <iostream>

#include "connection.hpp"
#include "error_exception.hpp"

/**
 * @brief basic connection test
 *
 * @param argc nuumber of command line parameters
 * @param argv value of the command line parameters
 * @return
 */
int main(int argc, char** argv)
{
  for (int cnt = 0; cnt < argc; cnt++) // NOLINT -altera-unroll-loops
    {
      std::cerr << "par[" << cnt << "] = '"
                << std::string(argv[cnt]) // NOLINT -cppcoreguidelines-pro-bounds-pointer-arithmetic
                << "'" << std::endl;
    }

  try
    {
      std::cerr << "connecting..." << std::endl;
      auto show = std::getenv("TRACE") != 0; // NOLINT -concurrency-mt-unsafe,readability-implicit-bool-conversion

      db::connection conn("test", "", "", &db::_log_, show);
      std::cerr << "connected" << std::endl;
      //int sts = 0;
    }
  catch (db::error_exception& e)
    {
      std::cerr << "cli error " << e << std::endl;
    }
  catch (...)
    {
      std::cerr << "unhandled exception " << std::endl;
    }
  return 0;
}
