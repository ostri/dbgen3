set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
set(DOXYGEN_GENERATE_MARKDOWN YES)
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_MAN NO)
set(DOXYGEN_MARKDOWN_SUPPORT YES)
set(DOXYGEN_EXTRACT_PRIVATE YES)
set(DOXYGEN_OUTPUT_DIRECTORY "/var/www/") # check http://192.168.56.106/index.html
doxygen_add_docs(docs
  "${CMAKE_SOURCE_DIR}/src"
  "${CMAKE_SOURCE_DIR}/mainpage.md"
WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/include"
COMMENT "dbgen3 documentation"
)
