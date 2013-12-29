cmake_minimum_required(VERSION 2.8.9)

project(tomate)

#set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS "-std=c++0x -g")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt4)
include(${QT_USE_FILE})

file(GLOB WIDGETS_SRC src/gui_widgets/*.cpp)
file(GLOB GUI_SRC src/gui_*.cpp)

add_executable(tomate_gui src/dataset.cpp ${GUI_SRC} ${WIDGETS_SRC} src/main.cpp src/xml.cpp lib/pugixml.cpp)
#qt4_use_modules(tomate_gui Widgets)
target_link_libraries(tomate_gui boost_date_time)
target_link_libraries(tomate_gui ${QT_LIBRARIES})

add_executable(test src/dataset.cpp src/test.cpp src/xml.cpp lib/pugixml.cpp)
target_link_libraries(test boost_unit_test_framework boost_date_time)

add_executable(test_gui src/test_gui.cpp src/dataset.cpp ${GUI_SRC} ${WIDGETS_SRC} src/xml.cpp lib/pugixml.cpp)
target_link_libraries(test_gui boost_unit_test_framework boost_date_time)
target_link_libraries(test_gui ${QT_LIBRARIES})
#qt4_use_modules(test_gui Widgets)

add_executable(test_gui2 src/test_gui2.cpp ${WIDGETS_SRC})
target_link_libraries(test_gui2 ${QT_LIBRARIES})
#qt4_use_modules(test_gui Widgets)
