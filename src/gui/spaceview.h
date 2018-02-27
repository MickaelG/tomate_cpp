
#ifndef SPACEVIEW_H
#define SPACEVIEW_H

#include <QGraphicsView>

class SpaceView: public QGraphicsView
{
    Q_OBJECT

public:
    SpaceView(QWidget* parent = nullptr);
    void zoom_fit();
    void resizeEvent(QResizeEvent *event);

private:
};

#endif //SPACEVIEW_H

