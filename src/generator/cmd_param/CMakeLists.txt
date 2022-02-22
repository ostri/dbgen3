# target_sources(proj_lib
# PRIVATE
#   cmdline_parameters.cpp
#   cmdline_parameters.hpp
# PUBLIC
#   cmdline_parameters.hpp
# )

# #find_library(BARRY_LIB barry)
# # This call requires CMake 3.13 or later
# target_link_libraries(proj_lib PUBLIC glog::glog gflags)

# # target_compile_definitions(proj_lib PUBLIC USE_BARRY)
# target_include_directories(proj_lib PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# set (LIB_NAME "cmd_param")
# add_library(${LIB_NAME} "")

# target_sources(${LIB_NAME}
#   PRIVATE
#     ${CMAKE_CURRENT_LIST_DIR}/cmdline_parameters.cpp
#   PUBLIC
#     ${CMAKE_CURRENT_LIST_DIR}/cmdline_parameters.hpp
#   )
# target_include_directories(${LIB_NAME}
#   PUBLIC
#     ${CMAKE_CURRENT_LIST_DIR}
#     ../utility_classes
#   )
