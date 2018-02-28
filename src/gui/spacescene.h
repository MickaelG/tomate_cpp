
#ifndef SPACESCENE_H
#define SPACESCENE_H

#include <QWidget>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenu>
#include <QDate>

#include <vector>
using namespace std;

class Crop;
class Crops;
class Plot;
class CropSelectionController;
class DateController;
class DatasetModel;
class DateRuler;

#include "geometry.h"


class CropSpaceRepr: public QGraphicsRectItem
{
public:
    CropSpaceRepr(Crop* p_crop, QDate date);
    Crop* get_pcrop();
    Rectangle get_rect() { return rect; }
private:
    Rectangle rect;
    Crop* p_crop;
};


class PlotRepresentation: public QGraphicsRectItem
{
public:
    PlotRepresentation(Crops& crops, Plot& plot, QDate date);
    void update_draw(QDate date);
    vector<CropSpaceRepr*> crop_reprs;

private:
    void update_draw(const vector<Crop*>& crops_to_plot, QDate date);
    void add_name();

    QMenu context_menu;
    Crops& crops;
    Plot& plot;
};


class SpaceScene: public QGraphicsScene
{
    Q_OBJECT

public:
    SpaceScene(DatasetModel& dataset_model,
               CropSelectionController& selection_controller,
               DateController& date_controller);
    void draw_scene();
    void clear_scene();
    Crop* getCropAtPos(QPointF scene_pos);
    void drawCropSelection();
    void removeCropSelection();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private slots:
    void selectCrop(Crop *p_crop);
    void update_draw();
    
private:
    DatasetModel& dataset_model;
    DateController& _date_controller;
    DateRuler* _date_ruler;
    CropSpaceRepr* selected_subd_repr;
    CropSelectionController& selection_controller;
    vector<PlotRepresentation*> plot_reprs;
};

#endif //SPACESCENE_H
