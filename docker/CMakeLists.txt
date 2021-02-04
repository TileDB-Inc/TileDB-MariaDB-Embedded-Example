cmake_minimum_required(VERSION 3.16)

project(Example)

SET(GCC_EXAMPLE_COMPILE_FLAGS "-Wno-write-strings")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_EXAMPLE_COMPILE_FLAGS}")

set(CMAKE_PREFIX_PATH "/home/ubuntu/mytile_server/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")    
find_package(MySQL)

add_executable(Example example.cxx)
target_link_libraries(Example ${MySQL_LIBRARIES})
