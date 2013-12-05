
#include "gui_mainwin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Dataset dataset;
    xml_read_data("../user_data/data.sfg", dataset);
    xml_write_data("../user_data/data_out.sfg", dataset);
    
    QApplication a(argc, argv);
    GuiMainWin w(dataset);
    w.show();

    return a.exec();
}