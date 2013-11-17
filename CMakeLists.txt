cmake_minimum_required(VERSION 2.8)

project(tomate)

set(CMAKE_CXX_FLAGS "-std=c++11")

add_executable(test src/dataset.cpp src/test.cpp src/xml.cpp lib/pugixml.cpp)
target_link_libraries(test boost_unit_test_framework boost_date_time)

