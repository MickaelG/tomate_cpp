

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

file(GLOB WIDGETS_SRC ./gui_widgets/*.cpp)

set(GUI_SRC
    croptimerepresentation.cpp
    DateRuler.cpp
    gui_calendar.cpp
    gui_controller.cpp
    gui_plants.cpp
    gui_plots.cpp
    gui_utils.cpp
    mainwindow.cpp
    spacescene.cpp
    spaceview.cpp
    timescene.cpp
    timeview.cpp
)

file(GLOB ui_FORMS gui_widgets/*.ui mainwindow.ui)
QT5_WRAP_UI(ui_FORMS_HEADERS ${ui_FORMS})

add_executable(tomate_gui ${GUI_SRC} ${WIDGETS_SRC} main.cpp ${qrc_outfile} ${ui_FORMS_HEADERS})
target_include_directories(tomate_gui PUBLIC ./ ./gui_widgets)
target_link_libraries(tomate_gui tomate_core)
target_include_directories(tomate_gui PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(tomate_gui boost_date_time boost_filesystem boost_system)
QT5_USE_MODULES(tomate_gui Widgets)
install(TARGETS tomate_gui RUNTIME DESTINATION bin)

add_executable(tomate_test_gui ./test/test_gui.cpp ${GUI_SRC} ${WIDGETS_SRC} ${ui_FORMS_HEADERS})
target_include_directories(tomate_test_gui PUBLIC ./ ./gui_widgets)
target_include_directories(tomate_test_gui PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(tomate_test_gui tomate_core)
target_link_libraries(tomate_test_gui boost_date_time boost_filesystem boost_system)
QT5_USE_MODULES(tomate_test_gui Widgets Test)
add_test(gui tomate_test_gui)

#Translation files
add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/tomate_fr.qm
                   COMMAND lrelease ${CMAKE_SOURCE_DIR}/tomate_fr.ts -qm ${CMAKE_BINARY_DIR}/tomate_fr.qm
                   MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/tomate_fr.ts)

set( qrc_outfile ${CMAKE_CURRENT_BINARY_DIR}/qrc_tomate.cxx)
set(qm_files ${CMAKE_BINARY_DIR}/tomate_fr.qm)
set(qrc_srcfile ${CMAKE_CURRENT_SOURCE_DIR}/tomate.qrc)
set(qrc_infile ${CMAKE_CURRENT_BINARY_DIR}/tomate_copied.qrc)
# Copy the QRC file to the output directory, because the files listed in the
# qrc file are relative to that directory.
add_custom_command(
    OUTPUT ${qrc_infile}
    COMMAND ${CMAKE_COMMAND} -E copy ${qrc_srcfile} ${qrc_infile}
    MAIN_DEPENDENCY ${qrc_srcfile}
    )
# Run the resource compiler (rcc_options should already be set). We can't
# use QT4_ADD_RESOURCES because the qrc file may not exist yet.
add_custom_command(
    OUTPUT ${qrc_outfile}
    COMMAND ${QT_RCC_EXECUTABLE}
    ARGS ${rcc_options} -name tomate -o ${qrc_outfile} ${qrc_infile}
    MAIN_DEPENDENCY ${qrc_infile}
    DEPENDS ${qm_files}
    )

