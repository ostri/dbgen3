#
# gsql_parser library
#
set(LIB gsql_parser)
set(incl_folder "${CMAKE_CURRENT_SOURCE_DIR}/${LIB}/")
add_library(${LIB} STATIC)

set (src_list
    core_parser
    gsql_q
    gsql_q_set
    gsql_qbuf_dscr
    gsql_sql_set
    gsql_sql_dscr
    xerces_strings
    xsd_grammar
    dom_error_handler
)
set (cpp_list ${src_list})
set (hpp_list ${src_list})
list(TRANSFORM cpp_list APPEND ".cpp")
list(TRANSFORM hpp_list APPEND ".hpp")
list(TRANSFORM cpp_list PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
list(TRANSFORM hpp_list PREPEND ${incl_folder})
message (STATUS "'${LIB}'     [${src_list}]")

target_sources(${LIB}
  PRIVATE
    ${cpp_list}
    ${hpp_list}
  PUBLIC
)
target_include_directories(${LIB}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE
    ${DB2_INCLUDE_DIRS}
  )
target_link_libraries(${LIB}
  PUBLIC
  PRIVATE
    ${RT_LIB}
    magic_enum::magic_enum
    fmt::fmt
    utility_classes
    db_integration
    exceptions
    cmd_param
    core
    gen
   )
