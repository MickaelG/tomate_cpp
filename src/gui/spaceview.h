
#ifndef SPACEVIEW_H
#define SPACEVIEW_H

#include <QGraphicsView>

class SpaceView: public QGraphicsView
{
    Q_OBJECT

public:
    SpaceView(QWidget* parent = nullptr);
    void zoom_fit();
    void wheelEvent(QWheelEvent* event);

private:
};

#endif //SPACEVIEW_H

