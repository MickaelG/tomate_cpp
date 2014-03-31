

#include <QMainWindow>

class Dataset;


class GuiMainWin : public QMainWindow
//class GuiMainWin : public QWidget
{
    Q_OBJECT

public:
    GuiMainWin(Dataset& dataset);
    ~GuiMainWin();
    
private:
    Dataset& dataset;
};
