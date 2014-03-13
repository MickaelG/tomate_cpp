
#include "OptionalCalendar.h"

#include <QVBoxLayout>
#include <QLabel>


OptionalCalendar::OptionalCalendar(QString title, QWidget* parent):
    QWidget(parent)
{
    QVBoxLayout* main_layout = new QVBoxLayout();
    this->setLayout(main_layout);
    QLabel* label = new QLabel(title);
    checkbox.setText(tr("Activate"));
    checkbox.setLayoutDirection(Qt::RightToLeft);
    checkbox.setChecked(true);
    QObject::connect(&checkbox, SIGNAL(stateChanged(int)), this, SLOT(updateState()));
    //calendar.setCalendarPopup(true);
    QHBoxLayout* title_layout = new QHBoxLayout();
    title_layout->addWidget(label);
    title_layout->addWidget(&checkbox);
    main_layout->addLayout(title_layout);
    main_layout->addWidget(&calendar);
}

void OptionalCalendar::setSelectedDate(QDate date)
{
    if (date.isValid())
    {
        calendar.setSelectedDate(date);
        //calendar.setDate(date);
        setEnabled();
    }
    else
    {
        setEnabled(false);
    }
}

QDate OptionalCalendar::selectedDate()
{
    if (checkbox.isChecked())
    {
        return calendar.selectedDate();
        //return calendar.date();
    }
    else
    {
        return QDate();
    }
}

void OptionalCalendar::setEnabled(bool enabled)
{
    if (enabled)
    {
        checkbox.setChecked(true);
        calendar.setEnabled(true);
    }
    else
    {
        checkbox.setChecked(false);
        calendar.setEnabled(false);
    }
}

void OptionalCalendar::updateState()
{
    if (checkbox.isChecked())
    {
        setEnabled();
    }
    else
    {
        setEnabled(false);
    }
}
