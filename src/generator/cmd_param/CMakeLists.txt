#
# utility_classes library
#
set(LIB cmd_param)
set(incl_folder "${CMAKE_CURRENT_SOURCE_DIR}/${LIB}/")
add_library(${LIB} STATIC)

set (src_list
  cmdline_parameters
)
set (cpp_list ${src_list})
set (hpp_list ${src_list})
list(TRANSFORM cpp_list APPEND ".cpp")
list(TRANSFORM hpp_list APPEND ".hpp")
list(TRANSFORM cpp_list PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
list(TRANSFORM hpp_list PREPEND ${incl_folder})
message (STATUS "'${LIB}'       [${src_list}]")

target_sources(${LIB}
  PRIVATE
    ${cpp_list}
  PUBLIC
    ${hpp_list}
)
target_include_directories(${LIB}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
#    ${fmt_SOURCE_DIR}/include
#    ${magic_enum_SOURCE_DIR}/include
  PRIVATE
)
target_link_libraries(${LIB}
  PUBLIC
  PRIVATE
    ${DB2_LIBRARIES}
    fmt::fmt
    magic_enum::magic_enum
    utility_classes
)
