
#include "gui/gui_mainwin.h"
#include "xml.h"
#include <sys/stat.h>
#include <QApplication>

#include <QTranslator>
#include <QLocale>
#include <QDateTime>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load("tomate_" + QLocale::system().name());
    a.installTranslator(&translator);

    //TODO: make it cross platform
    string home_dir;
    char const* temp = getenv("HOME");
    if(temp != NULL)
    {
        home_dir = string(temp);
    }
    string user_data_dir = home_dir + "/.config/tomate/";
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
        mkdir(user_data_dir.c_str(), 0755);
    }
    int wresult = xml_write_data(data_file, dataset);
    if (wresult != 0) {
        QMessageBox::critical(NULL, QObject::tr("Error"),
                              QObject::tr("Error writing data file."));
    }
    
    return result;
}

