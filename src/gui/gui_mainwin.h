
#ifndef GUI_MAINWIN_H
#define GUI_MAINWIN_H

#include "gui_controller.h"
#include "dataset.h"

#include <QObject>
#include <QMainWindow>
#include <QGraphicsView>

#include <string>

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
    GuiMainWin();
    ~GuiMainWin();
    
private:
    QWidget* createTabsWidget();
    void loadData();
    void writeData();
    virtual void closeEvent(QCloseEvent*);
    
    Dataset _dataset;
    DatasetModel dataset_model;
    DatasetController dataset_controller;
    CropSelectionController selection_controller;
    DateController date_controller;
    
    std::string _user_data_dir;
    std::string _data_file;
    bool _new_file;
};

#endif //GUI_MAINWIN_H
