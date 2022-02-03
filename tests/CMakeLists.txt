set(RT_LIB db3)
set (RT_SUBDIRS ${CMAKE_SOURCE_DIR}/src/runtime)
message(STATUS "DB2_LIB     ${DB2_FOUND} ${DB2_LIBRARIES}")
message(STATUS "source dir  ${CMAKE_SOURCE_DIR}")
message(STATUS "ME dir      ${magic_enum_SOURCE_DIR}")

add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/t1.hpp
  COMMAND dbgen3 --db_name test ${CMAKE_SOURCE_DIR}/gsql/t1.gsql --out_folder ${CMAKE_BINARY_DIR}
  DEPENDS dbgen3
  )
#-----------------------------------------------------------------------------
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
# ------------------------------------------------------------------------------------
add_executable(t1 
  test_1.cpp 
  ${CMAKE_SOURCE_DIR}/src/utility_classes/program_status.cpp
  ${magic_enum_SOURCE_DIR}/include/magic_enum.hpp
  ${CMAKE_BINARY_DIR}/t1.hpp
  )
target_compile_features(t1 PUBLIC cxx_std_20)
target_include_directories(t1 
  PRIVATE
    ${RT_SUBDIRS} 
    ${DB2_INCLUDE_DIRS} 
    ${magic_enum_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/utility_classes/
    ${fmt_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}
    )
target_link_libraries(t1
  PRIVATE
    fmt::fmt
    ${GLOG_LIBRARY}
    gflags
    ${DB2_LIBRARIES}
    ${RT_LIB}
    )
# ------------------------------------------------------------------------------------
#
# test_templ
#
add_executable(templ
  test_templ.cpp 
#  ${CMAKE_SOURCE_DIR}/src/utility_classes/program_status.cpp
  ${magic_enum_SOURCE_DIR}/include/magic_enum.hpp
)

target_compile_features(templ PUBLIC cxx_std_20)
target_include_directories(templ 
  PRIVATE
    ${RT_SUBDIRS} 
    ${DB2_INCLUDE_DIRS} 
    ${magic_enum_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/utility_classes/
    ${fmt_SOURCE_DIR}/include
    )
target_link_libraries(templ
  PRIVATE
    fmt::fmt
    ${GLOG_LIBRARY}
    gflags
    xerces-c
    ${DB2_LIBRARIES}
    ${RT_LIB}
)
