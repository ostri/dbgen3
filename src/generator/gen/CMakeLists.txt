#
# gen library
#
set(LIB gen)
set(incl_folder "${CMAKE_CURRENT_SOURCE_DIR}/${LIB}/")
add_library(${LIB} STATIC)

set (src_list
    gen
    gen_cpp
)
set (cpp_list ${src_list})
set (hpp_list ${src_list})
list(TRANSFORM cpp_list APPEND ".cpp")
list(TRANSFORM hpp_list APPEND ".hpp")
list(TRANSFORM cpp_list PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
list(TRANSFORM hpp_list PREPEND ${incl_folder})
message (STATUS "'${LIB}'             [${src_list}]")

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
)
target_link_libraries(${LIB}
  PUBLIC
  PRIVATE
    ${RT_LIB}
    fmt::fmt
    magic_enum::magic_enum
    utility_classes
    gsql_parser
    db_integration
    cmd_param
)
