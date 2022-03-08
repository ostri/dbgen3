#
# runtime library
#
set(RT_LIB db3)
set(incl_folder "${CMAKE_CURRENT_SOURCE_DIR}/runtime/")
add_library(${RT_LIB} STATIC)

set (src_list
  connection
  error
  error_dscr
  error_exception
  statement
  dbgen3_templ
  odbc_db
  serialize_templ
)
set (cpp_list ${src_list})
set (hpp_list ${src_list})
list(TRANSFORM cpp_list APPEND ".cpp")
list(TRANSFORM hpp_list APPEND ".hpp")
list(TRANSFORM cpp_list PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
list(TRANSFORM hpp_list PREPEND ${incl_folder})
message (STATUS "'${RT_LIB}' files: [${src_list}]")
message (STATUS "'db2_inclde_dirs' : [${DB2_INCLUDE_DIRS}]")

target_sources(${RT_LIB}
  PRIVATE
    ${cpp_list}
  PUBLIC
    ${hpp_list}
)
target_include_directories(${RT_LIB}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE
    ${DB2_INCLUDE_DIRS}
)
target_link_libraries(${RT_LIB}
  PUBLIC
  PRIVATE
    ${DB2_LIBRARIES}
  )
