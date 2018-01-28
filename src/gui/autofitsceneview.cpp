
#include "autofitsceneview.h"

AutofitSceneView::AutofitSceneView(QWidget* parent) :
    QGraphicsView(parent)
{
}

void AutofitSceneView::setHorizontalOnly()
{
    horizontal_only = true;
}

void AutofitSceneView::zoom_fit()
{
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}

void AutofitSceneView::resizeEvent(QResizeEvent *event)
{
    if (scene() == nullptr) {
        return;
    }

    int Margin = 10;
    if (horizontal_only) {
        QRectF itemsRect = scene()->itemsBoundingRect();
        int x = itemsRect.x();
        int y = itemsRect.y();
        int width = itemsRect.width();
        int height = itemsRect.height();
        setSceneRect(x - Margin, y - Margin,
                     width + 2 * Margin, height + 2 * Margin);
        setAlignment(Qt::AlignTop);
        setMinimumHeight(height + 2 * Margin);
    } else {
        zoom_fit();
    }
}

