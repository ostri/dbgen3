set(RT_LIB db3)
# set(PROJ_LIB proj)
set(PROG_NAME dbgen3)

add_subdirectory(utility_classes)
add_subdirectory(exceptions)
add_subdirectory(gsql_parser)
add_subdirectory(core)
add_subdirectory(db_integration)
add_subdirectory(cmd_param)
add_subdirectory(gen)


# add_library(${PROJ_LIB} STATIC)

# target_compile_features(${PROJ_LIB} PUBLIC
#   cxx_std_20
#   cxx_raw_string_literals
#   cxx_strong_enums
#   cxx_unicode_literals
#   cxx_auto_type
#   cxx_constexpr
# )
# target_link_libraries(${PROJ_LIB}
#   PRIVATE
#     ${RT_LIB}
# )
#------------------------------------------------------------------------------
configure_file(version.cpp.in version.cpp)
add_executable(${PROG_NAME}
  main.cpp
  ${GLOG_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}/version.cpp
)
target_include_directories(${PROG_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROG_NAME}
  PRIVATE
    # ${PROJ_LIB}
    ${RT_LIB}
    ${GLOG_LIBRARY}
    fmt::fmt
    magic_enum::magic_enum
    gflags
    xerces-c
    exceptions
    utility_classes
    core
    gsql_parser
    cmd_param
    db_integration
    gen
)
