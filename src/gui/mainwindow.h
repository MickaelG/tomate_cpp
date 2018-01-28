
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "gui_controller.h"
#include "dataset.h"

#include <QObject>
#include <QMainWindow>

#include <string>

class Dataset;

class MainWindow : public QMainWindow
//class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    void loadData();
    void writeData();
    virtual void closeEvent(QCloseEvent*);

    Ui::MainWindow *ui;

    Dataset _dataset;
    DatasetModel dataset_model;
    DatasetController dataset_controller;
    CropSelectionController selection_controller;
    DateController date_controller;
    
    std::string _user_data_dir;
    std::string _data_file;
    bool _new_file;
};

#endif //MAINWINDOW_H
