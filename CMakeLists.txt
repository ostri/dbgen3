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


  add_compile_options(
    -Wall
    -Wextra
    -pedantic
    -Werror
    -Wnarrowing
    -pthread
    # -fsanitize=address
    # -fno-omit-frame-pointer
    # -fsanitize=undefined
    # -fno-sanitize-recover=all
    # -fsanitize=float-divide-by-zero
    # -fsanitize=float-cast-overflow
    # -fno-sanitize=null
    # -fno-sanitize=alignment
    )
    add_link_options(
      # -fno-omit-frame-pointer
      # -fsanitize=address
    )
    # set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
    # set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

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

find_package (gflags REQUIRED)    # sudo dnf install gflags-devel
find_package (GLOG REQUIRED)      # sudo dnf install glog-devel
find_package (XercesC REQUIRED)   # sudo dnf install xerces-c-devel.x86_64
find_package (DB2 REQUIRED)
#find_package (doctest REQUIRED)   # sudo dnf install  doctest-devel.x86_64

# unit testing
FetchContent_Declare(DocTest GIT_REPOSITORY "https://github.com/doctest/doctest.git" GIT_TAG "2.4.7")
FetchContent_MakeAvailable(DocTest)

# Adds fmt::fmt
FetchContent_Declare(fmt GIT_REPOSITORY "https://github.com/fmtlib/fmt.git" GIT_TAG "8.1.1")
FetchContent_MakeAvailable(fmt)

# static enums
FetchContent_Declare(magic_enum GIT_REPOSITORY "https://github.com/Neargye/magic_enum.git" GIT_TAG v0.7.3)
FetchContent_MakeAvailable(magic_enum)

# The compiled library code is here
add_subdirectory(src)

# if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
#     set(OPTIONS
#     -Wall
#     -Wextra
#     -pedantic-errors
#     -Werror
#     -Wnarrowing
#     -pthread
#     -fsanitize=address
#     )
# elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
#     set(OPTIONS /W4 /WX)
#     if(HAS_PERMISSIVE_FLAG)
#         set(OPTIONS ${OPTIONS} /permissive-)
#     endif()
# endif()

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
  add_subdirectory(tests)
endif()

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

message(STATUS "fmt:*** '${fmtlib_SOURCE_DIR}/include'")
