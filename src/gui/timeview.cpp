
#include "timeview.h"

TimeView::TimeView(QWidget* parent) :
    QGraphicsView(parent)
{
}

void TimeView::resizeEvent(QResizeEvent *event)
{
    if (scene() == nullptr) {
        return;
    }

    int Margin = 10;

    QRectF itemsRect = scene()->itemsBoundingRect();
    int x = itemsRect.x();
    int y = itemsRect.y();
    int width = itemsRect.width();
    int height = itemsRect.height();
    setSceneRect(x - Margin, y - Margin,
                 width + 2 * Margin, height + 2 * Margin);
    setAlignment(Qt::AlignTop);
    setMinimumHeight(height + 2 * Margin);
}

