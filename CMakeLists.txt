cmake_minimum_required(VERSION 3.28)

project(puzzleLoop)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(main src/main.cpp)

add_library(reader STATIC src/readHtml.cpp)
add_library(solver STATIC src/solver.cpp)
add_library(interface STATIC src/interface.cpp)

target_link_libraries(main PRIVATE reader)
target_link_libraries(main PRIVATE solver)
target_link_libraries(main PRIVATE interface)
