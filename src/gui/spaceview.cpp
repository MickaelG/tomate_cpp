
#include <QWheelEvent>

#include "spaceview.h"

SpaceView::SpaceView(QWidget* parent) :
    QGraphicsView(parent)
{
}

void SpaceView::zoom_fit()
{
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}

void SpaceView::wheelEvent(QWheelEvent* event)
{
    const int degrees = event->delta() / 8;
    int steps = degrees / 15;

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    qreal scale_factor = 1;
    if (steps > 0) {
        scale_factor = steps * 1.1;
    } else {
        scale_factor = 1 / (-steps * 1.1);
    }

    scale(scale_factor, scale_factor);
}
