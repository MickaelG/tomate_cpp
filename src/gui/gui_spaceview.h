
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


class CropSpaceRepr: public QGraphicsRectItem
{
public:
    CropSpaceRepr(Crop* p_crop, QDate date);
    Crop* get_pcrop();
    Rectangle get_rect() { return rect; };
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
    QMenu context_menu;
    Crops& crops;
    Plot& plot;
};


class WholeScene: public QGraphicsScene
{
    Q_OBJECT

public:
    WholeScene(Dataset& dataset);
    void draw_scene();
    void clear_scene();
    Crop* getCropAtPos(QPointF scene_pos);
    void drawCropSelection();
    void removeCropSelection();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private slots:
    void set_date(QDate date);
    void selectCrop(Crop *p_crop);
    void update_draw();

signals:
    void crop_selected(Crop* p_crop);
    
private:
    Dataset& dataset;
    QDate date;
    Crop* selected_crop;
    CropSpaceRepr* selected_subd_repr;
    vector<PlotRepresentation*> plot_reprs;
};

class AutofitSceneView: public QGraphicsView
{
    Q_OBJECT
    
public:
    AutofitSceneView(QWidget* parent = nullptr);
    void zoom_fit();
    void resizeEvent(QResizeEvent *event);
};

