set(RT_LIB db3)
set (RT_SUBDIRS ${CMAKE_SOURCE_DIR}/src/runtime)
message(STATUS "DB2_LIB     ${DB2_FOUND} ${DB2_LIBRARIES}")
message(STATUS "source dir  ${CMAKE_SOURCE_DIR}")
message(STATUS "ME dir      ${magic_enum_SOURCE_DIR}")
add_executable(t0 
  test_0.cpp 
  ${CMAKE_SOURCE_DIR}/src/utility_classes/program_status.cpp
  ${magic_enum_SOURCE_DIR}/include/magic_enum.hpp
)

target_compile_features(t0 PUBLIC cxx_std_20)
target_include_directories(t0 
  PRIVATE
    ${RT_SUBDIRS} 
    ${DB2_INCLUDE_DIRS} 
    ${magic_enum_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/utility_classes/
    ${fmt_SOURCE_DIR}/include
    )
target_link_libraries(t0
  PRIVATE
    fmt::fmt
    ${GLOG_LIBRARY}
    gflags
    xerces-c
    ${DB2_LIBRARIES}
    ${RT_LIB}
)
