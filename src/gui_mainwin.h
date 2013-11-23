

#include <QMainWindow>
#include "dataset.h"


class GuiMainWin : public QMainWindow
//class GuiMainWin : public QWidget
{
    Q_OBJECT

    public:
        GuiMainWin(Dataset& data);
        ~GuiMainWin();
    private:
        Dataset& data;
};
