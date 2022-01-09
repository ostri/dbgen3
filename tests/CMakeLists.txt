set(RT_LIB db3)
set (RT_SUBDIRS ../src/runtime)
message(STATUS "DB2_LIB ${DB2_FOUND} ${DB2_LIBRARIES}")
add_executable(t0 test_0.cpp ../src/utility_classes/program_status.cpp)

target_compile_features(t0 PUBLIC cxx_std_20)
target_include_directories(t0 PUBLIC ${RT_SUBDIRS} ${DB2_INCLUDE_DIRS} ../src/utility_classes/)
target_link_libraries(t0
  PRIVATE
    ${GLOG_LIBRARY}
    gflags
    xerces-c
    ${DB2_LIBRARIES}
    ${RT_LIB}
)
