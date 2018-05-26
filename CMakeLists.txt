
cmake_minimum_required(VERSION 2.8.7)

project(tomate)

enable_testing()
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Widgets 5.5)

add_subdirectory(lib)
add_subdirectory(src/core)
add_subdirectory(src/gui)
add_subdirectory(src/cli)

install(PROGRAMS tomate.desktop DESTINATION share/applications/)

