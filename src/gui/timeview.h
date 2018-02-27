
#ifndef TIMEVIEW_H
#define TIMEVIEW_H

#include <QGraphicsView>

class TimeView: public QGraphicsView
{
    Q_OBJECT

public:
    TimeView(QWidget* parent = nullptr);
    void resizeEvent(QResizeEvent *event);

private:
};

#endif //TIMEVIEW_H

