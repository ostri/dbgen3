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
message (STATUS "'${RT_LIB}'             [${src_list}]")

target_sources(${RT_LIB}
  PRIVATE
    ${cpp_list}
    ${hpp_list}
  PUBLIC
)
set_target_properties(${RT_LIB}
    PROPERTIES
        DEBUG_POSTFIX _d
)
target_include_directories(${RT_LIB}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    ${DB2_INCLUDE_DIRS}
  PRIVATE
)
target_link_libraries(${RT_LIB}
  PUBLIC
  PRIVATE
    ${DB2_LIBRARIES}
  )

  install(TARGETS ${RT_LIB} EXPORT db3gen_inst)
  install(DIRECTORY runtime
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)