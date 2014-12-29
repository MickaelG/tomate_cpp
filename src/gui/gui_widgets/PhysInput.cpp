
#include "PhysInput.h"

#include <QValidator>
#include <QLabel>
#include <QGroupBox>
#include <QScrollArea>


PhysInput::PhysInput(QWidget* parent, Rectangle rect, bool editable):
    QWidget(parent)
{
    this->setLayout(new QHBoxLayout);

    x_widget = new QLineEdit();
    x_widget->setValidator( new QDoubleValidator(-1000, 1000, 2, this) );
    y_widget = new QLineEdit();
    y_widget->setValidator( new QDoubleValidator(-1000, 1000, 2, this) );
    width_widget = new QLineEdit();
    width_widget->setValidator( new QDoubleValidator(1, 1000, 2, this) );
    height_widget = new QLineEdit();
    height_widget->setValidator( new QDoubleValidator(1, 1000, 2, this) );
    x_widget->setEnabled(editable);
    y_widget->setEnabled(editable);
    width_widget->setEnabled(editable);
    height_widget->setEnabled(editable);
    this->layout()->addWidget(new QLabel(tr("x position (m):")));
    this->layout()->addWidget(x_widget);
    this->layout()->addWidget(new QLabel(tr("y position (m):")));
    this->layout()->addWidget(y_widget);
    this->layout()->addWidget(new QLabel(tr("width (m):")));
    this->layout()->addWidget(width_widget);
    this->layout()->addWidget(new QLabel(tr("height (m):")));
    this->layout()->addWidget(height_widget);

    set_shape(&rect);
}

Rectangle PhysInput::get_rect()
{
    //TODO: use float instead of ints. For now, all geometry data is stored in cm int
    int posx = int(x_widget->text().toDouble() * 100);
    int posy = int(y_widget->text().toDouble() * 100);
    int width = int(width_widget->text().toDouble() * 100);
    int height = int(height_widget->text().toDouble() * 100);
    return Rectangle(posx, posy, width, height);
}

void PhysInput::set_shape(const Shape* shape)
{
    if (shape)
    {
        x_widget->setText(QString::number(shape->get_min_x() / 100.0));
        y_widget->setText(QString::number(shape->get_min_y() / 100.0));
        width_widget->setText(QString::number(shape->get_width() / 100.0));
        height_widget->setText(QString::number(shape->get_height() / 100.0));
    }
    else
    {
        x_widget->setText("0");
        y_widget->setText("0");
        width_widget->setText("-1");
        height_widget->setText("-1");
    }
}
