set(PROJ_LIB proj_lib)
set(PROG_NAME dbgen3)
set(HEADER_LIST "")
set (SUBDIRS utility_classes cmd_param)

foreach (SUBDIR ${SUBDIRS})
    add_subdirectory(${SUBDIR})
    message( STATUS "  subdir: ${SUBDIR}")
endforeach()
message (STATUS "proj_lib '${PROJ_LIB}'")

# Make an automatic library - will be static or dynamic based on user setting
add_library(${PROJ_LIB})

# We need this directory, and users of our library will need it too
target_include_directories(${PROJ_LIB} PUBLIC ${SUBDIRS})

set (src_list
  utility_classes/common
  utility_classes/string_format
  utility_classes/log
  cmd_param/cmdline_parameters
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
  PUBLIC
    ${hpp_list}
)
## This depends on (header only) boost
#target_link_libraries(${PROJ_LIB} PRIVATE Boost::boost)

# All users of this library will need at least C++11
target_compile_features(${PROJ_LIB} PUBLIC cxx_std_20)
target_include_directories(${PROJ_LIB} PUBLIC ${SUBDIRS})
# IDEs should put the headers in a nice place
# source_group(
#   TREE "${PROJECT_SOURCE_DIR}/include"
#   PREFIX "Header Files"
#   FILES ${HEADER_LIST})

add_executable(${PROG_NAME} main.cpp ${hpp_list})
target_compile_features(${PROG_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROG_NAME} PUBLIC ${SUBDIRS})

target_link_libraries(${PROG_NAME}
  PRIVATE
    proj_lib
#    fmt::fmt
    glog::glog
    gflags
)
