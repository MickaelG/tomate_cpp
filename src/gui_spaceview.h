
#include <QWidget>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenu>
#include <QDate>
#include "dataset.h"


class WholeScene: public QGraphicsScene
{
    Q_OBJECT

public:
    WholeScene(Dataset& dataset);
    void set_date(QDate date);
    void update();
    
private:
    Dataset& dataset;
    void draw_scene();
    QDate date;
};

class WholeSceneView: public QGraphicsView
{
    Q_OBJECT
    
public:
    WholeSceneView(Dataset& dataset, QWidget* parent);
    void zoom_fit();
    void update();
    void resizeEvent(QResizeEvent *event);
private:
    WholeScene my_scene;
};

class SpaceViewWindow: public QWidget
{
    Q_OBJECT

    public:
        SpaceViewWindow(Dataset& data);
        void update();
    private:
        WholeSceneView view;
};


class PlotRepresentation: public QGraphicsRectItem
{
    public:
        PlotRepresentation(Crops& crops, Plot& plot, QDate date);
        void update(QDate date);
                
    private:
        QMenu context_menu;
        Crops& crops;
        Plot& plot;
};
                

class SubdRepresentation: public QGraphicsRectItem
{
    public:
        SubdRepresentation(Rectangle rect, Crop& crop, QDate date);
        Rectangle get_rect() { return rect; };
    private:
        Rectangle rect;
        Crop& crop;
};

