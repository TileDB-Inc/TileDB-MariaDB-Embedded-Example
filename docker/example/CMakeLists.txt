cmake_minimum_required(VERSION 3.16)

project(Example)

SET(GCC_EXAMPLE_COMPILE_FLAGS "-Wno-write-strings")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_EXAMPLE_COMPILE_FLAGS}")

find_path(MySQL_INCLUDE_PATH NAMES mysql/mysql.h)
message("MySQL include path " ${MySQL_INCLUDE_PATH})

find_library(MySQL_EMBEDDED_LIBRARIES NAMES mysqld)
message("MySQL embedded library path " ${MySQL_EMBEDDED_LIBRARIES})

add_executable(Example example.cxx)
target_include_directories(Example PUBLIC ${MySQL_INCLUDE_PATH})
target_link_libraries(Example ${MySQL_EMBEDDED_LIBRARIES})
