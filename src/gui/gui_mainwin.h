

#include <QMainWindow>
#include "dataset.h"


class GuiMainWin : public QMainWindow
//class GuiMainWin : public QWidget
{
    Q_OBJECT

public:
    GuiMainWin(Dataset& dataset);
    ~GuiMainWin();
    
private slots:
    void write_file();

private:
    Dataset& dataset;
};
