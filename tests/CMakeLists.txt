set(RT_LIB db3)

add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/t1.hpp
  COMMAND ${CMAKE_BINARY_DIR}/src/generator/dbgen3 --db_name test ${CMAKE_SOURCE_DIR}/gsql/t1.gsql --out_folder ${CMAKE_BINARY_DIR}
  DEPENDS ${CMAKE_BINARY_DIR}/src/generator/dbgen3 ${CMAKE_SOURCE_DIR}/gsql/t1.gsql
  )
#-----------------------------------------------------------------------------
add_executable(t0
  test_0.cpp
)
target_link_libraries(t0
  PRIVATE
    ${RT_LIB}
    fmt::fmt
    magic_enum::magic_enum
    utility_classes
    )
# ------------------------------------------------------------------------------------
add_executable(t1
  test_1.cpp
  ${CMAKE_BINARY_DIR}/t1.hpp
  )
target_include_directories(t1
  PRIVATE
    ${CMAKE_BINARY_DIR}
)
target_link_libraries(t1
  PRIVATE
    ${RT_LIB}
)
# ------------------------------------------------------------------------------------
#
# test_templ
#
add_executable(templ
  test_templ.cpp
)
target_include_directories(templ
  PRIVATE
)
target_link_libraries(templ
  PRIVATE
    ${RT_LIB}
    fmt::fmt
)
