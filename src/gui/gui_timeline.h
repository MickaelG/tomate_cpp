
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenu>
#include <QDate>
#include <QGraphicsItemGroup>

#include <vector>
using namespace std;

class Dataset;
class Crop;


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

private:
    void update_global_rect(const QGraphicsRectItem& in_rect);

    Crop& crop;
    QDate date0;
    QGraphicsRectItem* _global_rect;
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
    void clear_scene();
    CropTimeRepresentation* getCropReprAtPos(QPointF scene_pos);
    void selectCrop(CropTimeRepresentation *p_crop_repr);
    void selectNextCrop(bool reverse=false);
    void drawCropSelection();
    void removeCropSelection();
    //void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent* keyEvent);

public slots:
    void selectCrop(Crop *p_crop);
    void redraw();
    void previous_year();
    void next_year();

signals:
    void current_date_changed(QDate date);
    void crop_selected(Crop* p_crop);
    void size_changed();

private:
    Dataset& dataset;
    //QMenu* context_menu;
    QDate date;
    vector<CropTimeRepresentation*> crop_reprs;
    CropTimeRepresentation* selected_crop_repr;
    Crop* selected_crop;
    QGraphicsItem* _date_line;

    void add_year_buttons();
    void draw_date_line(QDate date, int bottom_y);
};

class WholeTimeSceneView: public QGraphicsView
{
    Q_OBJECT

public:
    WholeTimeSceneView(Dataset& dataset, QWidget* parent=NULL);
    WholeTimeScene* get_scene() { return _scene; }

private slots:
    void update_draw();
    void update_rect();

private:
    WholeTimeScene* _scene;
};
