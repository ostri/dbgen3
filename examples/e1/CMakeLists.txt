set (prog e1)
add_executable(${prog} ${prog}.cpp ${CMAKE_CURRENT_BINARY_DIR}/${prog}.hpp)
target_include_directories(${prog} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${prog} db3
)
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${prog}.hpp
  COMMAND
    ${CMAKE_BINARY_DIR}/src/generator/dbgen3
    --db_name test ${CMAKE_CURRENT_SOURCE_DIR}/${prog}.gsql
    --out_folder ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${CMAKE_BINARY_DIR}/src/generator/dbgen3 ${CMAKE_CURRENT_SOURCE_DIR}/${prog}.gsql
  )