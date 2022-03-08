#
# core library
#
set(LIB core)
set(incl_folder "${CMAKE_CURRENT_SOURCE_DIR}/${LIB}/")
add_library(${LIB} STATIC)

set (src_list
  executor
)
set (cpp_list ${src_list})
set (hpp_list ${src_list})
list(TRANSFORM cpp_list APPEND ".cpp")
list(TRANSFORM hpp_list APPEND ".hpp")
list(TRANSFORM cpp_list PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
list(TRANSFORM hpp_list PREPEND ${incl_folder})
message (STATUS "'${LIB}' files: [${src_list}]")
message (STATUS "'${LIB}' library : [${LIB}]")

target_sources(${LIB}
  PRIVATE
    ${cpp_list}
  PUBLIC
    ${hpp_list}
)
target_include_directories(${LIB}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${fmt_SOURCE_DIR}/include
    ${magic_enum_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/generator/db_integration
    ${CMAKE_SOURCE_DIR}/src/generator/gen
    ${CMAKE_SOURCE_DIR}/src/runtime
    # ${incl_folder}
    # ${CMAKE_SOURCE_DIR}/src/generator/exceptions
  PRIVATE
    ${DB2_INCLUDE_DIRS}
)
target_link_libraries(${LIB}
  PUBLIC
  PRIVATE
    ${DB2_LIBRARIES}
    exceptions
    fmt::fmt
    utility_classes
    gsql_parser
    cmd_param
    db_integration
    gen
  )
