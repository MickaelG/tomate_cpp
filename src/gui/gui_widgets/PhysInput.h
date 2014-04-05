
#ifndef GUI_PHYS_INPUT_H
#define GUI_PHYS_INPUT_H

#include <QLineEdit>
#include <QVBoxLayout>
#include "geometry.h"

class PhysInput: public QWidget
{
    Q_OBJECT

public:
    PhysInput(Rectangle rect=Rectangle(0, 0, 100, 100), bool editable=true, QWidget* parent=NULL);
    void set_shape(const Shape* shape);
    Rectangle get_rect();

private:
    QLineEdit* x_widget;
    QLineEdit* y_widget;
    QLineEdit* width_widget;
    QLineEdit* height_widget;
};

#endif //GUI_PHYS_INPUT_H
