
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


GuiMainWin::GuiMainWin(Dataset& dataset) :
    dataset_model(dataset),
    selection_controller(),
    dataset_controller(dataset_model, selection_controller)
{
    showMaximized();
    setWindowTitle("tomate");

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

GuiMainWin::~GuiMainWin()
{
    //TODO: delete tabwidget, timewidget, plantswidget, spacewidget
}

