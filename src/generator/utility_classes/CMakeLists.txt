#
# utility_classes library
#
set(LIB utility_classes)
set(incl_folder "${CMAKE_CURRENT_SOURCE_DIR}/${LIB}/")
add_library(${LIB} STATIC)

set (src_list
  common
  string_format
  log
  program_status
  multi_line
  enums
  )
set (cpp_list ${src_list})
set (hpp_list ${src_list})
list(TRANSFORM cpp_list APPEND ".cpp")
list(TRANSFORM hpp_list APPEND ".hpp")
list(TRANSFORM cpp_list PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
list(TRANSFORM hpp_list PREPEND ${incl_folder})
message (STATUS "'${LIB}' [${src_list}]")

target_sources(${LIB}
  PRIVATE
    ${cpp_list}
  PUBLIC
    ${hpp_list}
)
target_include_directories(${LIB}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE
    ${fmt_SOURCE_DIR}/include
    ${magic_enum_SOURCE_DIR}/include
)
target_link_libraries(${LIB}
  PUBLIC
  PRIVATE
    ${DB2_LIBRARIES}
    fmt::fmt
    magic_enum::magic_enum

)
