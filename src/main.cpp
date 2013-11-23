
#include "gui_mainwin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Dataset data;
    xml_read_data("user_data/data.sfg", data);
    xml_write_data("user_data/data_out.sfg", data);
    
    QApplication a(argc, argv);
    GuiMainWin w(data);
    w.show();

    return a.exec();
}