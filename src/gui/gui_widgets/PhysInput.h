
#ifndef GUI_PHYS_INPUT_H
#define GUI_PHYS_INPUT_H

#include <QLineEdit>
#include <QVBoxLayout>
#include "geometry.h"

class PhysInput: public QWidget
{
    Q_OBJECT

public:
    PhysInput(QWidget* parent=NULL, Rectangle rect=Rectangle(100, 100, 0, 0), bool editable=true);
    void set_shape(const Shape* shape);
    Rectangle get_rect();

private:
    QLineEdit* x_widget;
    QLineEdit* y_widget;
    QLineEdit* width_widget;
    QLineEdit* height_widget;
};

#endif //GUI_PHYS_INPUT_H
