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

  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(PREBUILT_MODULE_PATH ${CMAKE_BINARY_DIR}/modules)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # find_program(
  #   CLANG_TIDY_EXE
  #   NAMES "clang-tidy"
  #   DOC "Path to clang-tidy executable"
  # )
  # set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

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

# # Formatting library
# FetchContent_Declare(fmtlib GIT_REPOSITORY https://github.com/fmtlib/fmt.git GIT_TAG 5.3.0)
# FetchContent_MakeAvailable(fmtlib)
# # Adds fmt::fmt

# The compiled library code is here
add_subdirectory(src)

# # The executable code is here
# add_subdirectory(apps)

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
  add_subdirectory(tests)
endif()
