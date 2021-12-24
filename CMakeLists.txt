

cmake_minimum_required(VERSION 3.16)
project(dbgen3 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(PREBUILT_MODULE_PATH ${CMAKE_BINARY_DIR}/modules)

find_package (gflags REQUIRED)
find_package (glog 0.6.0 REQUIRED)


#add_compile_options(-fmodules-ts)
#add_compile_options(-stdlib=libc++)
#add_compile_options(-fbuiltin-module-map)
#dd_compile_options(-fimplicit-module-maps)
#add_compile_options(-fprebuilt-module-path=${PREBUILT_MODULE_PATH})

include(CTest)
enable_testing()

add_executable(dbgen3 src/main.cpp)
target_link_libraries (dbgen3 glog::glog)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
