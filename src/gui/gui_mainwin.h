
#include "gui_controller.h"

#include <QObject>
#include <QMainWindow>
#include <QGraphicsView>

class Dataset;

class AutofitSceneView: public QGraphicsView
{
    Q_OBJECT

public:
    AutofitSceneView(bool horizontal_only, QWidget* parent = nullptr);
    void zoom_fit();
    void resizeEvent(QResizeEvent *event);

private:
    bool horizontal_only;
};

class GuiMainWin : public QMainWindow
//class GuiMainWin : public QWidget
{
    Q_OBJECT

public:
    GuiMainWin(Dataset& dataset);
    ~GuiMainWin();
    
private:
    QWidget* createTabsWidget();
    DatasetModel dataset_model;
    DatasetController dataset_controller;
    CropSelectionController selection_controller;
    DateController date_controller;
};
