
#ifndef AUTOFITSCENEVIEW_H
#define AUTOFITSCENEVIEW_H

#include <QGraphicsView>

class AutofitSceneView: public QGraphicsView
{
    Q_OBJECT

public:
    AutofitSceneView(QWidget* parent = nullptr);
    void setHorizontalOnly();
    void zoom_fit();
    void resizeEvent(QResizeEvent *event);

private:
    bool horizontal_only = false;
};

#endif //AUTOFITSCENEVIEW_H

