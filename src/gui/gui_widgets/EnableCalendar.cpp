#include "EnableCalendar.h"
#include "ui_EnableCalendar.h"

EnableCalendar::EnableCalendar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnableCalendar)
{
    ui->setupUi(this);
    QObject::connect(ui->checkbox, SIGNAL(stateChanged(int)), this, SLOT(updateState()));
    QObject::connect(ui->calendar, SIGNAL(dateChanged(QDate)), this, SIGNAL(dateChanged(QDate)));
}

EnableCalendar::~EnableCalendar()
{
    delete ui;
}


void EnableCalendar::setSelectedDate(QDate date)
{
    if (date.isValid())
    {
        //ui->calendar->setSelectedDate(date);
        ui->calendar->setDate(date);
        setEnabled();
    }
    else
    {
        setEnabled(false);
    }
}

QDate EnableCalendar::selectedDate()
{
    if (ui->checkbox->isChecked())
    {
        //return ui->calendar->selectedDate();
        return ui->calendar->date();
    }
    else
    {
        return QDate();
    }
}

void EnableCalendar::setEnabled(bool enabled)
{
    if (enabled)
    {
        ui->checkbox->setChecked(true);
        ui->calendar->setEnabled(true);
    }
    else
    {
        ui->checkbox->setChecked(false);
        ui->calendar->setEnabled(false);
    }
}

void EnableCalendar::updateState()
{
    if (ui->checkbox->isChecked())
    {
        setEnabled();
    }
    else
    {
        setEnabled(false);
    }
}
