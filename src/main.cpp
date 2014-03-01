
#include "gui/gui_mainwin.h"
#include <QApplication>

#include <QTranslator>
#include <QLocale>
#include <QDateTime>

int main(int argc, char *argv[])
{
    //TODO: make it cross platform
    string user_data_dir = "../user_data/";
    string data_file = user_data_dir + "/data.sfg";
    
    Dataset dataset;
    xml_read_data(data_file, dataset);
    
    QApplication a(argc, argv);
    
    QTranslator translator;
    translator.load("tomate_" + QLocale::system().name());
    a.installTranslator(&translator);
    
    GuiMainWin w(dataset);
    w.show();

    int result = a.exec();
    
    //Save the data file before writing the new one
    string suffix = QDateTime::currentDateTime().toString(".yyyyMMdd'_'hh'h'mm").toStdString();
    std::ifstream src(data_file, std::ios::binary);
    std::ofstream dst(data_file + suffix, std::ios::binary);
    dst << src.rdbuf();
    
    xml_write_data(data_file, dataset);
    
    return result;
}

