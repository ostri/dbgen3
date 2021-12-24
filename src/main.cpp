#include <gflags/gflags.h>
#include <iostream>


int main(int argc, char **argv)
{
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  std::cout << "Hello, world!\n";
}
