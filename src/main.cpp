
#include "gui/gui_mainwin.h"
#include "xml.h"
#include "dataset.h"

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDateTime>
#include <QMessageBox>

#include <boost/filesystem.hpp>
#include <fstream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load("tomate_" + QLocale::system().name(), ":/lang/");
    a.installTranslator(&translator);

    //TODO: make it cross platform
    string data_home;
    char const* temp = getenv("XDG_DATA_HOME");
    if (temp != NULL)
    {
        data_home = string(temp);
    }
    else
    {
        string home_dir;
        char const* temp = getenv("HOME");
        if(temp != NULL)
        {
            home_dir = string(temp);
        } else {
            QMessageBox::critical(NULL, QObject::tr("Error"),
                                  QObject::tr("Error, HOME environment variable not set."));
            return -1;
        }
        data_home =  home_dir + "/.local/share/";

    }

    string user_data_dir = data_home + "/tomate/";
    string data_file = user_data_dir + "/data.sfg";

    Dataset dataset;
    int res = xml_read_data(data_file, dataset);
    bool new_file = false;
    if (res == -1)
    {
        new_file = true;
        QMessageBox::information(NULL, QObject::tr("Information"),
                                 QObject::tr("Data file not found. A new one will be created"));
    } else if (res != 0)
    {
        QMessageBox::critical(NULL, QObject::tr("Error"),
                              QObject::tr("Error reading data file. Please check console output."));
        return -1;
    }

    GuiMainWin w(dataset);
    w.show();

    int result = a.exec();
    
    //Save the data file before writing the new one
    string suffix = QDateTime::currentDateTime().toString(".yyyyMMdd'_'hh'h'mm").toStdString();
    std::ifstream src(data_file, std::ios::binary);
    std::ofstream dst(data_file + suffix, std::ios::binary);
    dst << src.rdbuf();
    
    if (new_file)
    {
        boost::filesystem::create_directories(user_data_dir.c_str());
    }
    int wresult = xml_write_data(data_file, dataset);
    if (wresult != 0) {
        QMessageBox::critical(NULL, QObject::tr("Error"),
                              QObject::tr("Error writing data file."));
    }
    
    return result;
}

