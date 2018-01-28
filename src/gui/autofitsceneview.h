
#ifndef AUTOFITSCENEVIEW_H
#define AUTOFITSCENEVIEW_H

#include <QGraphicsView>

class AutofitSceneView: public QGraphicsView
{
    Q_OBJECT

public:
    AutofitSceneView(bool horizontal_only, QWidget* parent = nullptr);
    void zoom_fit();
    void resizeEvent(QResizeEvent *event);

private:
    bool horizontal_only;
};

#endif //AUTOFITSCENEVIEW_H

