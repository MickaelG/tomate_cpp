
#include "PhysInput.h"

#include <QValidator>
#include <QLabel>
#include <QGroupBox>


PhysInput::PhysInput(Rectangle rect, bool editable, QWidget* parent):
    QWidget(parent)
{
    this->setLayout(new QHBoxLayout);

    x_widget = new QLineEdit();
    x_widget->setValidator( new QIntValidator(-10000, 10000, this) );
    y_widget = new QLineEdit();
    y_widget->setValidator( new QIntValidator(-10000, 10000, this) );
    width_widget = new QLineEdit();
    width_widget->setValidator( new QIntValidator(1, 10000, this) );
    height_widget = new QLineEdit();
    height_widget->setValidator( new QIntValidator(1, 10000, this) );
    x_widget->setEnabled(editable);
    y_widget->setEnabled(editable);
    width_widget->setEnabled(editable);
    height_widget->setEnabled(editable);
    this->layout()->addWidget(new QLabel(tr("x position:")));
    this->layout()->addWidget(x_widget);
    this->layout()->addWidget(new QLabel(tr("y position:")));
    this->layout()->addWidget(y_widget);
    this->layout()->addWidget(new QLabel(tr("width:")));
    this->layout()->addWidget(width_widget);
    this->layout()->addWidget(new QLabel(tr("height:")));
    this->layout()->addWidget(height_widget);

    set_shape(&rect);
}

Rectangle PhysInput::get_rect()
{
    int posx = x_widget->text().toInt();
    int posy = y_widget->text().toInt();
    int width = width_widget->text().toInt();
    int height = height_widget->text().toInt();
    return Rectangle(width, height, posx, posy);
}

void PhysInput::set_shape(const Shape* shape)
{
    x_widget->setText(QString::number(shape->get_min_x()));
    y_widget->setText(QString::number(shape->get_min_y()));
    width_widget->setText(QString::number(shape->get_width()));
    height_widget->setText(QString::number(shape->get_height()));
}
