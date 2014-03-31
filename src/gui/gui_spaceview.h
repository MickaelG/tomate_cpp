
#include <QWidget>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenu>
#include <QDate>

#include <vector>
using namespace std;

class Dataset;
class Crop;
class Crops;
class Plot;
#include "geometry.h"


class SubdRepresentation: public QGraphicsRectItem
{
public:
    SubdRepresentation(Rectangle rect, Crop& crop, QDate date);
    Crop* get_pcrop();
    Rectangle get_rect() { return rect; };
private:
    Rectangle rect;
    Crop& crop;
};


class PlotRepresentation: public QGraphicsRectItem
{
public:
    PlotRepresentation(Crops& crops, Plot& plot, QDate date);
    void update_draw(QDate date);
    vector<SubdRepresentation*> subd_reprs;

private:
    QMenu context_menu;
    Crops& crops;
    Plot& plot;
};


class WholeScene: public QGraphicsScene
{
    Q_OBJECT

public:
    WholeScene(Dataset& dataset);
    void update_draw();
    void draw_scene();
    void clear_scene();
    Crop* getCropAtPos(QPointF scene_pos);
    void drawCropSelection();
    void removeCropSelection();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private slots:
    void set_date(QDate date);
    void selectCrop(Crop *p_crop);

signals:
    void crop_selected(Crop* p_crop);
    
private:
    Dataset& dataset;
    QDate date;
    Crop* selected_crop;
    SubdRepresentation* selected_subd_repr;
    vector<PlotRepresentation*> plot_reprs;
};

class WholeSceneView: public QGraphicsView
{
    Q_OBJECT
    
public:
    WholeSceneView(Dataset& dataset, QWidget* parent);
    void zoom_fit();
    void update_draw();
    void resizeEvent(QResizeEvent *event);
    WholeScene* get_scene() { return &my_scene; };

private:
    WholeScene my_scene;
};

class SpaceViewWindow: public QWidget
{
    Q_OBJECT

public:
    SpaceViewWindow(Dataset& data);
    WholeSceneView* get_view() { return &view; };
     
private slots:
    void update_draw();

private:
    WholeSceneView view;
};

