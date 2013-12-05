
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenu>

#include "dataset.h"
#include "gui_widgets.h"


class ContextMenu: public QMenu
{
    Q_OBJECT
public:
    ContextMenu();
    void show(QPoint pos, Crop* crop);

signals:
    void modify_selected(Crop* crop);
    void new_selected();

private:
    Crop* crop;
};

class CropTimeRepresentation: public QGraphicsItemGroup
{
public:
    CropTimeRepresentation(Crop& crop, QMenu* context_menu, QDate date0 = QDate(), QWidget* parent = NULL);
    void create_rect(QDate start_date, QDate end_date, bool planned=false);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void delete_me();
    void show(QPoint point, CropTimeRepresentation* crop_repr);
private:
    Crop& crop;
    QDate date0;
    ContextMenu* context_menu;
};


class PlotTimeRepresentation: public QGraphicsItemGroup
{
public:
    PlotTimeRepresentation(Crops& crops, Plot& plot, QMenu* context_menu, QDate date0, QWidget* parent=NULL);
    void update(Crops& crops, Plot& plot);
private:
    Crops& crops;
    Plot& plot;
    QDate date0;
    QMenu* context_menu;
};

class MonthsRepresentation: public QGraphicsItemGroup
{
public:
    MonthsRepresentation(QDate date_start, QDate date_end, QWidget* parent=NULL);
private:
};


class WholeTimeScene: public QGraphicsScene
{
  Q_OBJECT

public:
    WholeTimeScene(Dataset& dataset, QWidget* parent=NULL);
    void draw_scene();
    void clear_crops();

public slots:
    void redraw();
    void previous_year();
    void next_year();

private:
    Dataset& dataset;
    EditCropDialog edit_crop_dialog;
    ContextMenu* context_menu;
    int year;
    
    void add_year_buttons();
};

class WholeTimeSceneView: public QGraphicsView
{
    Q_OBJECT

public:
    WholeTimeSceneView(Dataset& dataset, QWidget* parent=NULL);
    void update_draw();
    void update_rect();

private:
};

class TimelineWindow: public QWidget
{
    Q_OBJECT

public:
    TimelineWindow(Dataset& dataset, QWidget* parent=NULL);
    void update();

private:
    WholeTimeSceneView view;
};