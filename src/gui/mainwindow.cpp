
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dataset.h"
#include "calendar.h"
#include "gui_plants.h"
#include "gui_plots.h"
#include "gui_spaceview.h"
#include "timescene.h"
#include "gui_calendar.h"
#include "gui_controller.h"
#include "PlantsModel.h"
#include "PlotsModel.h"
#include "EditCropWidget.h"
#include "autofitsceneview.h"

#include "xml.h"

#include <QWidget>
#include <QGridLayout>
#include <QToolBar>
#include <QDockWidget>
#include <QMessageBox>
#include <QCloseEvent>

#include <boost/filesystem.hpp>


void MainWindow::writeData()
{
    if (_new_file)
    {
        boost::filesystem::create_directories(_user_data_dir.c_str());
    }
    int wresult = xml_write_data(_data_file, dataset_model.get_dataset());
    if (wresult != 0) {
        QMessageBox::critical(NULL, QObject::tr("Error"),
                              QObject::tr("Error writing data file."));
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    writeData();
    event->accept();
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _dataset(),
    dataset_model(_dataset),
    dataset_controller(dataset_model, selection_controller),
    selection_controller(),
    _new_file(false)
{
    ui->setupUi(this);

    showMaximized();
    
    loadData();

    PlantsModel* plants_model = new PlantsModel(dataset_model.get_dataset().get_plants());
    PlotsModel* plots_model = new PlotsModel(dataset_model.get_dataset().get_plots());

    PlantsWindow* plantswidget = new PlantsWindow(plants_model);
    PlotsWindow* plotswidget = new PlotsWindow(plots_model);

    EditCropWidget* edit_crop_widget = new EditCropWidget(dataset_model, dataset_controller,
                                                          selection_controller, plants_model, plots_model);

    ui->toolBar->addAction(tr("Add Crop"), edit_crop_widget, SLOT(show_add()));
    ui->toolBar->addAction(tr("Plants"), plantswidget, SLOT(show()));
    ui->toolBar->addAction(tr("Plots"), plotswidget, SLOT(show()));

    TimeScene* timescene = new TimeScene(dataset_model,
                                         selection_controller,
                                         date_controller);
    ui->timeView->setHorizontalOnly();
    ui->timeView->setScene(timescene);
    
    SpaceScene* spacescene = new SpaceScene(dataset_model,
                                            selection_controller,
                                            date_controller);
    ui->spaceView->setScene(spacescene);

    QObject::connect(edit_crop_widget->ui->EditPlantsBtn, SIGNAL(clicked()), plantswidget, SLOT(show()));
    QObject::connect(edit_crop_widget->ui->EditPlotsBtn, SIGNAL(clicked()), plotswidget, SLOT(show()));

    ui->centralLayout->addWidget(edit_crop_widget);
}

void MainWindow::loadData()
{
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
            return;
        }
        data_home =  home_dir + "/.local/share/";

    }

    _user_data_dir = data_home + "/tomate/";
    _data_file = _user_data_dir + "/data.sfg";

    int res = xml_read_data(_data_file, dataset_model.get_dataset());
    if (res == -1)
    {
        _new_file = true;
        QMessageBox::information(NULL, QObject::tr("Information"),
                                 QObject::tr("Data file not found. A new one will be created"));
    } else if (res != 0)
    {
        QMessageBox::critical(NULL, QObject::tr("Error"),
                              QObject::tr("Error reading data file. Please check console output."));
        return;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

