
#include "dataset.h"
#include "calendar.h"
#include "gui_mainwin.h"
#include "gui_plants.h"
#include "gui_plots.h"
#include "gui_spaceview.h"
#include "gui_timeline.h"
#include "gui_calendar.h"
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

    WholeScene* spacescene = new WholeScene(data);
    AutofitSceneView* spaceview = new AutofitSceneView();
    spaceview->setScene(spacescene);
    tab_widget->addTab(spaceview, QObject::tr("Space view"));


    WholeTimeSceneView* timeview = new WholeTimeSceneView(data);
    tab_widget->addTab(timeview, QObject::tr("Time view"));

    //CalendarScene* calendarscene = new CalendarScene(CropsCalendar(data.get_crops()));
    //AutofitSceneView* calendarview = new AutofitSceneView();
    //calendarview->setScene(calendarscene);
    //tab_widget->addTab(calendarview, QObject::tr("Calendar"));

    PlantsWindow* plantswidget = new PlantsWindow(plants_model);
    PlotsWindow* plotswidget = new PlotsWindow(plots_model);

    EditCropWidget* edit_crop_widget = new EditCropWidget(data, plants_model, plots_model);

    QObject::connect(plantswidget, SIGNAL(timeline_need_update()), spacescene, SLOT(update_draw()));
    QObject::connect(plantswidget, SIGNAL(timeline_need_update()), timeview, SLOT(update_draw()));

    QObject::connect(plotswidget, SIGNAL(timeline_need_update()), spacescene, SLOT(update_draw()));
    QObject::connect(plotswidget, SIGNAL(timeline_need_update()), timeview, SLOT(update_draw()));

    QObject::connect(edit_crop_widget, SIGNAL(dataset_changed()), timeview, SLOT(update_draw()));
    QObject::connect(edit_crop_widget, SIGNAL(dataset_changed()), spacescene, SLOT(update_draw()));

    QObject::connect(edit_crop_widget, SIGNAL(select_crop(Crop*)), timeview->get_scene(), SLOT(selectCrop(Crop*)));
    QObject::connect(edit_crop_widget, SIGNAL(select_crop(Crop*)), spacescene, SLOT(selectCrop(Crop*)));

    //Date of the spacewidget
    QObject::connect(timeview->get_scene(), SIGNAL(current_date_changed(QDate)), spacescene, SLOT(set_date(QDate)));

    //Crops selection synchronisation
    QObject::connect(timeview->get_scene(), SIGNAL(crop_selected(Crop*)), edit_crop_widget, SLOT(set_crop_values(Crop*)));
    QObject::connect(spacescene, SIGNAL(crop_selected(Crop*)), edit_crop_widget, SLOT(set_crop_values(Crop*)));
    QObject::connect(timeview->get_scene(), SIGNAL(crop_selected(Crop*)),
                     spacescene, SLOT(selectCrop(Crop*)));
    QObject::connect(spacescene, SIGNAL(crop_selected(Crop*)),
                     timeview->get_scene(), SLOT(selectCrop(Crop*)));

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
    setWindowTitle("tomate");
    
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(new QGridLayout);
    centralWidget()->layout()->addWidget(createTabsWidget(dataset));
}

GuiMainWin::~GuiMainWin() {
    //TODO: delete tabwidget, timewidget, plantswidget, spacewidget
}

