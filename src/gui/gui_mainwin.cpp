
#include "dataset.h"
#include "gui_mainwin.h"
#include "gui_plants.h"
#include "gui_plots.h"
#include "gui_spaceview.h"
#include "gui_timeline.h"
#include "PlantsModel.h"
#include "PlotsModel.h"
#include "EditCropWidget.h"

#include "xml.h"

#include <QGridLayout>
#include <QToolBar>

QWidget* createTabsWidget(Dataset& data)
{
    PlantsModel* plants_model = new PlantsModel(data.get_plants());
    PlotsModel* plots_model = new PlotsModel(data.get_plots());

    QTabWidget* tab_widget = new QTabWidget;

    SpaceViewWindow* spacewidget = new SpaceViewWindow(data);
    tab_widget->addTab(spacewidget, QObject::tr("Space view"));

    TimelineWindow* timewidget = new TimelineWindow(data);
    tab_widget->addTab(timewidget, QObject::tr("Time view"));

    PlantsWindow* plantswidget = new PlantsWindow(plants_model);
    PlotsWindow* plotswidget = new PlotsWindow(plots_model);

    EditCropWidget* edit_crop_widget = new EditCropWidget(data, plants_model, plots_model);

    QObject::connect(plantswidget, SIGNAL(timeline_need_update()), spacewidget, SLOT(update_draw()));
    QObject::connect(plantswidget, SIGNAL(timeline_need_update()), timewidget, SLOT(update_draw()));

    QObject::connect(plotswidget, SIGNAL(timeline_need_update()), spacewidget, SLOT(update_draw()));
    QObject::connect(plotswidget, SIGNAL(timeline_need_update()), timewidget, SLOT(update_draw()));

    QObject::connect(edit_crop_widget, SIGNAL(dataset_changed()), timewidget, SLOT(update_draw()));
    QObject::connect(edit_crop_widget, SIGNAL(dataset_changed()), spacewidget, SLOT(update_draw()));

    QObject::connect(edit_crop_widget, SIGNAL(select_crop(Crop*)), timewidget->get_view()->get_scene(), SLOT(selectCrop(Crop*)));
    QObject::connect(edit_crop_widget, SIGNAL(select_crop(Crop*)), spacewidget->get_view()->get_scene(), SLOT(selectCrop(Crop*)));

    //Date of the spacewidget
    QObject::connect(timewidget->get_view()->get_scene(), SIGNAL(current_date_changed(QDate)), spacewidget->get_view()->get_scene(), SLOT(set_date(QDate)));

    //Crops selection synchronisation
    QObject::connect(timewidget->get_view()->get_scene(), SIGNAL(crop_selected(Crop*)), edit_crop_widget, SLOT(set_crop_values(Crop*)));
    QObject::connect(spacewidget->get_view()->get_scene(), SIGNAL(crop_selected(Crop*)), edit_crop_widget, SLOT(set_crop_values(Crop*)));
    QObject::connect(timewidget->get_view()->get_scene(), SIGNAL(crop_selected(Crop*)),
                     spacewidget->get_view()->get_scene(), SLOT(selectCrop(Crop*)));
    QObject::connect(spacewidget->get_view()->get_scene(), SIGNAL(crop_selected(Crop*)),
                     timewidget->get_view()->get_scene(), SLOT(selectCrop(Crop*)));

    QObject::connect(edit_crop_widget->ui->EditPlantsBtn, SIGNAL(clicked()), plantswidget, SLOT(show()));
    QObject::connect(edit_crop_widget->ui->EditPlotsBtn, SIGNAL(clicked()), plotswidget, SLOT(show()));

    QWidget* widget = new QWidget;
    QGridLayout* main_layout = new QGridLayout;
    main_layout->addWidget(tab_widget);
    main_layout->addWidget(edit_crop_widget);
    widget->setLayout(main_layout);

    return widget;
}


GuiMainWin::GuiMainWin(Dataset& dataset) : dataset(dataset) {
    showMaximized();
    
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(new QGridLayout);
    centralWidget()->layout()->addWidget(createTabsWidget(dataset));
}

GuiMainWin::~GuiMainWin() {
    //TODO: delete tabwidget, timewidget, plantswidget, spacewidget
}

