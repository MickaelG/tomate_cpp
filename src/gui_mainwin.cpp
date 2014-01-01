#include "gui_mainwin.h"
#include "gui_plants.h"
#include "gui_spaceview.h"
#include "gui_timeline.h"

#include <QGridLayout>
#include <QToolBar>

QTabWidget* createTabsWidget(Dataset& data)
{
    QTabWidget* widget = new QTabWidget;

    SpaceViewWindow* spacewidget = new SpaceViewWindow(data);
    widget->addTab(spacewidget, QObject::tr("Space view"));

    TimelineWindow* timewidget = new TimelineWindow(data);
    widget->addTab(timewidget, QObject::tr("Time view"));

    PlantsWindow* plantswidget = new PlantsWindow(data.get_plants());
    widget->addTab(plantswidget, QObject::tr("Plants"));

    //self.plantswidget.timeline_need_update.connect(self.timewidget.update)
    //self.plantswidget.timeline_need_update.connect(self.spacewidget.update)
    
    //self.spacewidget.view.scene().edit_crop_dialog.timeline_need_update.connect(self.spacewidget.update)
    //self.spacewidget.view.scene().edit_crop_dialog.timeline_need_update.connect(self.timewidget.update)
    
    QObject::connect(timewidget->get_view()->get_scene()->get_ecd(), SIGNAL(dataset_changed()), timewidget, SLOT(update()));
    QObject::connect(timewidget->get_view()->get_scene()->get_ecd(), SIGNAL(dataset_changed()), spacewidget, SLOT(update()));

    return widget;
}


GuiMainWin::GuiMainWin(Dataset& dataset) : dataset(dataset) {
    showMaximized();
    
    /*
    QToolBar* toolbar = new QToolBar();
    QAction* write_action = toolbar->addAction(tr("Write"));
    QObject::connect(write_action, SIGNAL(triggered()), this, SLOT(write_file()));
    addToolBar(toolbar);
    */
    
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(new QGridLayout);
    centralWidget()->layout()->addWidget(createTabsWidget(dataset));
}

void GuiMainWin::write_file()
{
    xml_write_data("../user_data/data_out.sfg", dataset);
}


GuiMainWin::~GuiMainWin() {
    //TODO: delete tabwidget, timewidget, plantswidget, spacewidget
}

