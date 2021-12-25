

# Works with 3.11 and tested through 3.22
cmake_minimum_required(VERSION 3.14...3.22)

project(
  dbgen3
  VERSION 0.1
  DESCRIPTION "database access code genrator"
  LANGUAGES CXX
)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here

  # Let's ensure -std=c++xx instead of -std=g++xx
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(PREBUILT_MODULE_PATH ${CMAKE_BINARY_DIR}/modules)

  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # Testing only available if this is the main app
  include(CTest)

  # Docs only available if this is the main app
  find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)
  if(Doxygen_FOUND)
    add_subdirectory(docs)
  else()
    message(STATUS "Doxygen not found, not building docs")
  endif()
endif()


include(FetchContent)
#include(cmake/add_FetchContent_MakeAvailable.cmake)

find_package (gflags REQUIRED)
find_package (glog 0.6.0 REQUIRED)

# Formatting library
FetchContent_Declare(fmtlib GIT_REPOSITORY https://github.com/fmtlib/fmt.git GIT_TAG 5.3.0)
FetchContent_MakeAvailable(fmtlib)
# Adds fmt::fmt

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(apps)

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
  add_subdirectory(tests)
endif()


# include(GNUInstallDirs)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
#   ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
#   ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
#   ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})


# add_subdirectory(src)

# include(CTest)
# enable_testing()

# # add_executable (
# #     dbgen3
# #     src/main.cpp
# #     src/utility_classes/common.cpp
# #     src/utility_classes/string_format.cpp
# #     src/utility_classes/cmdline_parameters.cpp
# # )
# # target_link_libraries (dbgen3 glog::glog gflags)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
