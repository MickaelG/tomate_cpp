
#ifndef GUI_PHYS_INPUT_H
#define GUI_PHYS_INPUT_H

#include <QLineEdit>
#include <QVBoxLayout>
#include "geometry.h"

class LineEditDist: public QLineEdit
{
    Q_OBJECT
public:
    LineEditDist(QWidget* parent=NULL);
    float getValueCm() const;
    void setValueCm(float val);

};

class PhysInput: public QWidget
{
    Q_OBJECT

public:
    PhysInput(QWidget* parent=NULL, Rectangle rect=Rectangle(0, 0, 100, 100), bool editable=true);
    void set_shape(const Shape& shape);
    Rectangle get_rect();

private:
    LineEditDist* x_widget;
    LineEditDist* y_widget;
    LineEditDist* width_widget;
    LineEditDist* height_widget;
};

#endif //GUI_PHYS_INPUT_H
