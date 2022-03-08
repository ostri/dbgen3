set(RT_LIB db3)
set(PROJ_LIB proj)
set(PROG_NAME dbgen3)

#message(STATUS "fmt: '${fmt_SOURCE_DIR}/include'")
add_subdirectory(utility_classes)
add_subdirectory(exceptions)
add_subdirectory(gsql_parser)
add_subdirectory(core)
add_subdirectory(db_integration)
add_subdirectory(cmd_param)
add_subdirectory(gen)

set (src_list
  # utility_classes/common
  # utility_classes/string_format
  # utility_classes/log
  # utility_classes/program_status
  # utility_classes/multi_line
  # utility_classes/enums

  #cmd_param/cmdline_parameters

  # gsql_parser/core_parser
  # gsql_parser/gsql_q
  # gsql_parser/gsql_q_set
  # gsql_parser/gsql_qbuf_dscr
  # gsql_parser/gsql_sql_set
  # gsql_parser/gsql_sql_dscr
  # gsql_parser/xerces_strings
  # gsql_parser/xsd_grammar
  # gsql_parser/dom_error_handler

#  core/executor

  # db_integration/db2_reader
  # db_integration/fld_dscr

  # gen/gen
  # gen/gen_cpp
)
set (cpp_list ${src_list})
set (hpp_list ${src_list})
list(TRANSFORM cpp_list APPEND ".cpp")
list(TRANSFORM hpp_list APPEND ".hpp")
message (STATUS "src_list: ${src_list}")
message (STATUS "fmt: [${fmtlib_SOURCE_DIR}]")

add_library(${PROJ_LIB} STATIC)

target_compile_features(${PROJ_LIB} PUBLIC
  cxx_std_20
  cxx_raw_string_literals
  cxx_strong_enums
  cxx_unicode_literals
  cxx_auto_type
  cxx_constexpr
)
target_include_directories(${PROJ_LIB}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    utility_classes
    gsql_parser
    cmd_param
    core
    db_integration
    gen
    exceptions
    ../runtime
    ${DB2_INCLUDE_DIRS}
)

target_sources(${PROJ_LIB}
  PRIVATE
    ${cpp_list}
    ${hpp_list}
    ${rt_hpp_list}
    ${magic_enum_SOURCE_DIR}/include/magic_enum.hpp
)
target_link_libraries(${PROJ_LIB}
  PRIVATE
    fmt::fmt
    magic_enum::magic_enum
    ${DB2_LIBRARIES}
    ${RT_LIB}
    core
)
#------------------------------------------------------------------------------
configure_file(version.cpp.in version.cpp)
add_executable(${PROG_NAME} main.cpp
#  ${hpp_list}
  ${GLOG_INCLUDE_DIRS}
  ${magic_enum_SOURCE_DIR}/include/magic_enum.hpp
  ${CMAKE_CURRENT_BINARY_DIR}/version.cpp
)
#target_compile_features(${PROG_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROG_NAME}
  PRIVATE
#    cmd_param
#    gsql_parser
#    utility_classes
    cmd_param
#    core
#    db_integration
#    gen
#    exceptions
#    ../runtime
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${DB2_INCLUDE_DIRS}
    ${fmt_SOURCE_DIR}/include
    ${magic_enum_SOURCE_DIR}/include
)

target_link_libraries(${PROG_NAME}
  PRIVATE
    ${PROJ_LIB}
    ${RT_LIB}
    fmt::fmt
    ${GLOG_LIBRARY}
    gflags
    xerces-c
    exceptions
    utility_classes
    core
    gsql_parser
)
