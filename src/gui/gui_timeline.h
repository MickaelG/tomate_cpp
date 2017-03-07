
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenu>
#include <QDate>
#include <QGraphicsItemGroup>

#include <vector>

class DatasetModel;
class Crop;
class CropSelectionController;
class DateController;
class DateRuler;


class CropTimeRepresentation: public QGraphicsItemGroup
{
public:
    CropTimeRepresentation(Crop &crop, float ypos, float height,
                           QDate date0 = QDate(), QWidget* parent = NULL);
    QGraphicsRectItem* create_rect(QDate start_date, QDate end_date,
                                   float ypos, float height, bool planned=false);
    void delete_me();
    const Crop *get_pcrop() const;
    Crop *get_pcrop();
    //void show(QPoint point, Crop* p_crop);
    void set_selected(bool val);
    void _date_ruler(QDate end_date, QDate date0);

private:
    void update_global_rect(const QGraphicsRectItem& in_rect);

    Crop& crop;
    QDate date0;
    QGraphicsRectItem* _global_rect;
};


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
    void crop_selected(Crop* p_crop);
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
};
