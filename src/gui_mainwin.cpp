#include "gui_mainwin.h"
#include "gui_plants.h"
#include "gui_spaceview.h"
#include "gui_timeline.h"

#include <QGridLayout>

QTabWidget* createTabsWidget(Dataset& data)
{
    QTabWidget* widget = new QTabWidget;

    SpaceViewWindow* spacewidget = new SpaceViewWindow(data);
    widget->addTab(spacewidget, "Space view");

    TimelineWindow* timewidget = new TimelineWindow(data);
    widget->addTab(timewidget, "Time view");

    PlantsWindow* plantswidget = new PlantsWindow(data.get_plants());
    widget->addTab(plantswidget, "Plants");

    //self.plantswidget.timeline_need_update.connect(self.timewidget.update)
    //self.plantswidget.timeline_need_update.connect(self.spacewidget.update)
    //self.timewidget.view.scene().edit_crop_dialog.timeline_need_update.connect(self.spacewidget.update)
    //self.spacewidget.view.scene().edit_crop_dialog.timeline_need_update.connect(self.spacewidget.update)
    //self.timewidget.view.scene().edit_crop_dialog.timeline_need_update.connect(self.timewidget.update)
    //self.spacewidget.view.scene().edit_crop_dialog.timeline_need_update.connect(self.timewidget.update)

    return widget;
}



GuiMainWin::GuiMainWin(Dataset& data) : data(data) {
    this->showMaximized();
    this->setCentralWidget(new QWidget);
    this->centralWidget()->setLayout(new QGridLayout);
    this->centralWidget()->layout()->addWidget(createTabsWidget(data));
}
GuiMainWin::~GuiMainWin() {
    //TODO: delete tabwidget, timewidget, plantswidget, spacewidget
}

