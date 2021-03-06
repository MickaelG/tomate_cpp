
#ifndef GUI_TIMESCENE_H
#define GUI_TIMESCENE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenu>
#include <QDate>
#include <QGraphicsItemGroup>

#include <vector>

#include "croptimerepresentation.h"

class DatasetModel;
class Crop;
class CropSelectionController;
class DateController;
class DateRuler;


class TimeScene: public QGraphicsScene
{
  Q_OBJECT

public:
    TimeScene(DatasetModel& dataset,
              CropSelectionController& selection_controller,
              DateController& _date_controller,
              QWidget* parent=NULL);
    void draw_scene();
    void clear_scene();
    CropTimeRepresentation* getCropReprAtPos(QPointF scene_pos);
    void selectNextCrop(bool reverse=false);
    void drawCropSelection();
    void removeCropSelection();
    //void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent* keyEvent);

public slots:
    void selectCrop(Crop *p_crop);
    void redraw();
    void date_changed(bool year_changed);

signals:
    void size_changed();

private:
    DatasetModel& dataset_model;
    //QMenu* context_menu;
    std::vector<CropTimeRepresentation*> crop_reprs;
    CropTimeRepresentation* selected_crop_repr;
    CropSelectionController& _selection_controller;
    DateController& _date_controller;
    DateRuler* _date_ruler;

    void draw_date_line(QDate date, int bottom_y);
    void add_crops();
};

#endif //GUI_TIMESCENE_H
