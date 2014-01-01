
#include "OptionalCalendar.h"

#include <QVBoxLayout>
#include <QLabel>


OptionalCalendar::OptionalCalendar(QString title, QWidget* parent):
    QWidget(parent)
{
    this->setLayout(new QVBoxLayout);
    QLabel* label = new QLabel(title);
    checkbox.setText(tr("Activate"));
    checkbox.setChecked(true);
    //QObject::connect(&checkbox, SIGNAL(stateChanged(int)), &calendar, SLOT(setEnabled(int)));
    this->layout()->addWidget(label);
    this->layout()->addWidget(&calendar);
    this->layout()->addWidget(&checkbox);
}

void OptionalCalendar::setSelectedDate(QDate date)
{
    if (date.isValid())
    {
        calendar.setSelectedDate(date);
        checkbox.setChecked(true);
    }
    else
    {
        checkbox.setChecked(false);
    }
}

QDate OptionalCalendar::selectedDate()
{
    if (checkbox.isChecked())
    {
        return calendar.selectedDate();
    }
    else
    {
        return QDate();
    }
}
