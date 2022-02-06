set(PROJ_LIB proj)
set(RT_LIB db3)
set(PROG_NAME dbgen3)
set(HEADER_LIST "")
set (RT_SUBDIRS runtime)
set (SUBDIRS utility_classes cmd_param gsql_parser exceptions db_integration core gen ${RT_SUBDIRS} )

foreach (SUBDIR ${SUBDIRS})
    add_subdirectory(${SUBDIR})
    message( STATUS "  subdir: ${SUBDIR}")
endforeach()
message (STATUS "proj_lib '${PROJ_LIB}'")

add_library(${PROJ_LIB} OBJECT)

target_include_directories(${PROJ_LIB} PUBLIC ${SUBDIRS} ${DB2_INCLUDE_DIRS})

set (src_list
  utility_classes/common
  utility_classes/string_format
  utility_classes/log
  utility_classes/program_status
  utility_classes/multi_line
  utility_classes/enums

  cmd_param/cmdline_parameters

  gsql_parser/core_parser
  gsql_parser/gsql_q
  gsql_parser/gsql_q_set
  gsql_parser/gsql_qbuf_dscr
  gsql_parser/gsql_sql_set
  gsql_parser/gsql_sql_dscr
  gsql_parser/xerces_strings

  core/executor

  db_integration/db2_reader
  db_integration/fld_dscr

  gen/gen
  gen/gen_cpp
)
set (cpp_list ${src_list})
set (hpp_list ${src_list})
list(TRANSFORM cpp_list APPEND ".cpp")
list(TRANSFORM hpp_list APPEND ".hpp")
message (STATUS "${cpp_list}")
message (STATUS "${hpp_list}")

target_sources(${PROJ_LIB}
  PRIVATE
    ${cpp_list}
    ${hpp_list}
    ${rt_hpp_list}
    ${magic_enum_SOURCE_DIR}/include/magic_enum.hpp
  PUBLIC
    ${hpp_list}
    ${rt_hpp_list}
)
target_include_directories(${PROJ_LIB} 
  PUBLIC 
    ${SUBDIRS}
    ${DB2_INCLUDE_DIRS}
    ${magic_enum_SOURCE_DIR}/include
    ${fmt_SOURCE_DIR}/include/
)

# ## This depends on (header only) boost
# target_link_libraries(${PROJ_LIB} PRIVATE ${RT_LIB})

# # All users of this library will need at least C++11
# target_compile_features(${PROJ_LIB} PUBLIC cxx_std_20)
# target_include_directories(${PROJ_LIB} PUBLIC ${SUBDIRS})
# # IDEs should put the headers in a nice place
# # source_group(
# #   TREE "${PROJECT_SOURCE_DIR}/include"
# #   PREFIX "Header Files"
# #   FILES ${HEADER_LIST})

add_library(${RT_LIB})

set (rt_src_list
  runtime/connection
  runtime/error
  runtime/error_dscr
  runtime/error_exception
  runtime/statement
  runtime/dbgen3_templ
  runtime/odbc_db
  runtime/serialize_templ
)
set (rt_cpp_list ${rt_src_list})
set (rt_hpp_list ${rt_src_list} )
list(TRANSFORM rt_cpp_list APPEND ".cpp")
list(TRANSFORM rt_hpp_list APPEND ".hpp")
message (STATUS "${rt_cpp_list}")
message (STATUS "${rt_hpp_list}")

target_sources(${RT_LIB}
  PRIVATE
    ${rt_cpp_list}
    ${rt_hpp_list}
    ${magic_enum_SOURCE_DIR}/include/magic_enum.hpp
  PUBLIC
    ${rt_hpp_list}
)
message(STATUS "fmt           ${fmt_SOURCE_DIR}")
message(STATUS "magic_enum    ${magic_enum_SOURCE_DIR}")
message(STATUS "db2 incl dirm ${DB2_INCLUDE_DIRS}")
target_compile_features(${RT_LIB} PUBLIC cxx_std_20)
#target_include_directories(${RT_LIB} PRIVATE ${RT_SUBDIRS} ${DB2_INCLUDE_DIRS} ${fmtlib_SOURCE_DIR}/include/fmt/)
target_include_directories(${RT_LIB} 
  PRIVATE 
    ${RT_SUBDIRS} 
    ${DB2_INCLUDE_DIRS} 
    ${magic_enum_SOURCE_DIR}/include
    ${fmtlib_SOURCE_DIR}/include
)
target_link_libraries(${RT_LIB} 
  PRIVATE 
    fmt::fmt 
    ${DB2_LIBRARIES}
)

#------------------------------------------------------------------------------
configure_file(version.cpp.in version.cpp)
add_executable(${PROG_NAME} main.cpp 
  ${hpp_list} 
  ${GLOG_INCLUDE_DIRS} 
  ${magic_enum_SOURCE_DIR}/include/magic_enum.hpp
  ${CMAKE_CURRENT_BINARY_DIR}/version.cpp
)
target_compile_features(${PROG_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROG_NAME} 
  PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SUBDIRS} 
    ${DB2_INCLUDE_DIRS} 
    ${fmtlib_SOURCE_DIR}/include
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
)
