
#include "spaceview.h"

SpaceView::SpaceView(QWidget* parent) :
    QGraphicsView(parent)
{
}

void SpaceView::zoom_fit()
{
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}

void SpaceView::resizeEvent(QResizeEvent *event)
{
    if (scene() == nullptr) {
        return;
    }

    zoom_fit();
}
