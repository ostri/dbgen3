#
# runtime library
#
set(RT_LIB db3)
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
list(TRANSFORM hpp_list PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/runtime/")
message (STATUS "'${RT_LIB}' files: [${src_list}]")

target_sources(${RT_LIB}
  PRIVATE
    ${cpp_list}
  PUBLIC
    ${hpp_list}
)
#target_compile_features(${RT_LIB} PUBLIC cxx_std_20)
target_include_directories(${RT_LIB}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/runtime
  PRIVATE
    ${DB2_INCLUDE_DIRS}
)
target_link_libraries(${RT_LIB}
  PUBLIC
  PRIVATE
    ${DB2_LIBRARIES}
  )
