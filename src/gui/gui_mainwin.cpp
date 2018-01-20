
#include "dataset.h"
#include "calendar.h"
#include "gui_mainwin.h"
#include "gui_plants.h"
#include "gui_plots.h"
#include "gui_spaceview.h"
#include "gui_timeline.h"
#include "gui_calendar.h"
#include "gui_controller.h"
#include "PlantsModel.h"
#include "PlotsModel.h"
#include "EditCropWidget.h"

#include "xml.h"

#include <QWidget>
#include <QGridLayout>
#include <QToolBar>
#include <QDockWidget>
#include <QMessageBox>
#include <QCloseEvent>

#include <boost/filesystem.hpp>

AutofitSceneView::AutofitSceneView(bool horizontal_only, QWidget* parent) :
    QGraphicsView(parent), horizontal_only(horizontal_only)
{
}

void AutofitSceneView::zoom_fit()
{
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}

void AutofitSceneView::resizeEvent(QResizeEvent *event)
{
    if (scene() == nullptr) {
        return;
    }

    int Margin = 20;
    if (horizontal_only) {
        QRectF itemsRect = scene()->itemsBoundingRect();
        int x = itemsRect.x();
        int y = itemsRect.y();
        int width = itemsRect.width();
        int height = itemsRect.height();
        setSceneRect(x - Margin, y - Margin,
                     width + 2 * Margin, height + 2 * Margin);
        setAlignment(Qt::AlignTop);
    } else {
        zoom_fit();
    }
}

QWidget* GuiMainWin::createTabsWidget()
{
    QTabWidget* tab_widget = new QTabWidget;

    SpaceScene* spacescene = new SpaceScene(dataset_model,
                                            selection_controller,
                                            date_controller);
    AutofitSceneView* spaceview = new AutofitSceneView(false);
    spaceview->setScene(spacescene);
    tab_widget->addTab(spaceview, QObject::tr("Space view"));


    TimeScene* timescene = new TimeScene(dataset_model,
                                         selection_controller,
                                         date_controller);
    AutofitSceneView* timeview = new AutofitSceneView(true);
    timeview->setScene(timescene);
    tab_widget->addTab(timeview, QObject::tr("Time view"));

    return tab_widget;
}

void GuiMainWin::writeData()
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

void GuiMainWin::closeEvent(QCloseEvent* event)
{
    writeData();
    event->accept();
}


GuiMainWin::GuiMainWin() :
    _dataset(),
    dataset_model(_dataset),
    dataset_controller(dataset_model, selection_controller),
    selection_controller(),
    _new_file(false)
{
    showMaximized();
    setWindowTitle("tomate");
    
    loadData();

    PlantsModel* plants_model = new PlantsModel(dataset_model.get_dataset().get_plants());
    PlotsModel* plots_model = new PlotsModel(dataset_model.get_dataset().get_plots());

    PlantsWindow* plantswidget = new PlantsWindow(plants_model);
    PlotsWindow* plotswidget = new PlotsWindow(plots_model);

    EditCropWidget* edit_crop_widget = new EditCropWidget(dataset_model, dataset_controller,
                                                          selection_controller, plants_model, plots_model);

    QToolBar* toolbar = new QToolBar("", this);
    addToolBar(toolbar);
    toolbar->addAction(tr("Add Crop"), edit_crop_widget, SLOT(show_add()));
    toolbar->addAction(tr("Plants"), plantswidget, SLOT(show()));
    toolbar->addAction(tr("Plots"), plotswidget, SLOT(show()));
    toolbar->setMovable(false);

    QWidget* central_widget = new QWidget(this);
    QVBoxLayout* central_layout = new QVBoxLayout;
    central_widget->setLayout(central_layout);
    central_layout->addWidget(createTabsWidget());

    setCentralWidget(central_widget);

    QObject::connect(edit_crop_widget->ui->EditPlantsBtn, SIGNAL(clicked()), plantswidget, SLOT(show()));
    QObject::connect(edit_crop_widget->ui->EditPlotsBtn, SIGNAL(clicked()), plotswidget, SLOT(show()));

    central_layout->addWidget(edit_crop_widget);
}

void GuiMainWin::loadData()
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

GuiMainWin::~GuiMainWin()
{
    //TODO: delete tabwidget, timewidget, plantswidget, spacewidget
}

