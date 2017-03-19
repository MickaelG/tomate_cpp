
#include "gui/gui_mainwin.h"

#include <QApplication>
#include <QTranslator>
#include <QLocale>

#include <boost/filesystem.hpp>
#include <fstream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load("tomate_" + QLocale::system().name(), ":/lang/");
    a.installTranslator(&translator);

    GuiMainWin w;
    w.show();

    int result = a.exec();
    
    return result;
}

