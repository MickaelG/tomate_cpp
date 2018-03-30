
find_package(Boost 1.53 COMPONENTS date_time filesystem system unit_test_framework REQUIRED)

file(GLOB CORE_SRC *.cpp)
set (CORE_TEST_SRC ./test/test.cpp ./test/testgeometry.cpp)

add_library(tomate_core ${CORE_SRC})
target_link_libraries(tomate_core pugixml)
target_include_directories(tomate_core PUBLIC ./)
QT5_USE_MODULES(tomate_core Core)

add_executable(tomate_test ${CORE_TEST_SRC})

target_link_libraries(tomate_test tomate_core)
target_link_libraries(tomate_test boost_unit_test_framework)
target_link_libraries(tomate_test boost_date_time)

add_test(NAME core
         COMMAND tomate_test
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

