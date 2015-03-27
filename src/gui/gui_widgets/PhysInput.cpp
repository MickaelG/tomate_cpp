
#include "PhysInput.h"

#include <QValidator>
#include <QLabel>
#include <QGroupBox>
#include <QScrollArea>

class DistValidator: public QValidator {
public:
    void fixup(QString &input) const override
    {
        if (input.simplified() == "m") {
            input = "0 m";
        }
    }

    State validate(QString & input, int & pos) const override
    {
        if (input.simplified() == "m") {
            return State::Intermediate;
        }
        QRegExp val_rx("-?[\\d.]+\\s*m?");
        if (val_rx.exactMatch(input)) {
            if (!input.endsWith("m")) {
                input = input.simplified() + " m";
            }
            return State::Acceptable;
        }
        return State::Invalid;
    }
};

LineEditDist::LineEditDist(QWidget *parent) : QLineEdit(parent)
{
    this->setValidator(new DistValidator());
}

float LineEditDist::getValueCm() const {
    QString text = this->text();
    text = text.replace("m", "");
    return text.toDouble() * 100;
}

void LineEditDist::setValueCm(float val) {
    this->setText(QString::number(val / 100.0) + " m");
}

PhysInput::PhysInput(QWidget* parent, Rectangle rect, bool editable):
    QWidget(parent)
{
    this->setLayout(new QHBoxLayout);

    x_widget = new LineEditDist();
    y_widget = new LineEditDist();
    width_widget = new LineEditDist();
    height_widget = new LineEditDist();
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
    //TODO: use float instead of ints. For now, all geometry data is stored in cm int
    int posx = int(x_widget->getValueCm());
    int posy = int(y_widget->getValueCm());
    int width = int(width_widget->getValueCm());
    int height = int(height_widget->getValueCm());
    return Rectangle(posx, posy, width, height);
}

void PhysInput::set_shape(const Shape* shape)
{
    if (shape)
    {
        x_widget->setValueCm(shape->get_min_x());
        y_widget->setValueCm(shape->get_min_y());
        width_widget->setValueCm(shape->get_width());
        height_widget->setValueCm(shape->get_height());
    }
    else
    {
        x_widget->setText("0 m");
        y_widget->setText("0 m");
        width_widget->setText("-1 m");
        height_widget->setText("-1 m");
    }
}
