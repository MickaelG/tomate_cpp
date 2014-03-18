cmake_minimum_required(VERSION 2.8.9)

project(tomate)

#set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS "-std=c++0x -g")


# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories( ${CMAKE_SOURCE_DIR}/src/core/ ${CMAKE_SOURCE_DIR}/lib/
                     ${CMAKE_SOURCE_DIR}/src/gui/ )
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Boost 1.54 COMPONENTS date_time unit_test_framework)

find_package(Qt4 4.8)
include(${QT_USE_FILE})

file(GLOB WIDGETS_SRC src/gui/gui_widgets/*.cpp)
file(GLOB GUI_SRC src/gui/*.cpp)
file(GLOB CORE_SRC src/core/*.cpp lib/pugixml.cpp)

add_executable(tomate_gui ${CORE_SRC} ${GUI_SRC} ${WIDGETS_SRC} src/main.cpp)
#qt4_use_modules(tomate_gui Widgets)
target_link_libraries(tomate_gui boost_date_time)
target_link_libraries(tomate_gui ${QT_LIBRARIES})

add_executable(test ${CORE_SRC} src/test/test.cpp)
target_link_libraries(test boost_unit_test_framework boost_date_time)

add_executable(test_gui src/test_gui.cpp ${CORE_SRC} ${GUI_SRC} ${WIDGETS_SRC})
target_link_libraries(test_gui boost_unit_test_framework boost_date_time)
target_link_libraries(test_gui ${QT_LIBRARIES})
#qt4_use_modules(test_gui Widgets)

#Installation
install(TARGETS tomate_gui RUNTIME DESTINATION bin)
install(PROGRAMS tomate.desktop DESTINATION share/applications/)

