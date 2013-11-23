cmake_minimum_required(VERSION 2.8.9)

project(tomate)

set(CMAKE_CXX_FLAGS "-std=c++11")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)
add_executable(tomate_gui src/dataset.cpp src/gui_widgets.cpp src/gui_plants.cpp src/gui_spaceview.cpp src/gui_timeline.cpp src/gui_mainwin.cpp src/main.cpp src/xml.cpp lib/pugixml.cpp)
qt5_use_modules(tomate_gui Widgets)
target_link_libraries(tomate_gui boost_date_time)

add_executable(test src/dataset.cpp src/test.cpp src/xml.cpp lib/pugixml.cpp)
target_link_libraries(test boost_unit_test_framework boost_date_time)

add_executable(test_gui src/test_gui.cpp src/dataset.cpp src/gui_widgets.cpp src/gui_plants.cpp src/gui_spaceview.cpp src/gui_timeline.cpp src/xml.cpp lib/pugixml.cpp)
target_link_libraries(test_gui boost_unit_test_framework boost_date_time)
qt5_use_modules(test_gui Widgets)