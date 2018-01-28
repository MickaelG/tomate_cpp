
#include "autofitsceneview.h"

AutofitSceneView::AutofitSceneView(bool horizontal_only, QWidget* parent) :
    QGraphicsView(parent), horizontal_only(horizontal_only)
{
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

    int Margin = 20;
    if (horizontal_only) {
        QRectF itemsRect = scene()->itemsBoundingRect();
        int x = itemsRect.x();
        int y = itemsRect.y();
        int width = itemsRect.width();
        int height = itemsRect.height();
        setSceneRect(x - Margin, y - Margin,
                     width + 2 * Margin, height + 2 * Margin);
        setAlignment(Qt::AlignTop);
    } else {
        zoom_fit();
    }
}

