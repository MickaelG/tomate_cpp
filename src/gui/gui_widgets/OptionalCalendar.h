
#ifndef GUI_OPTIONALCALENDAR_H
#define GUI_OPTIONALCALENDAR_H

#include <QCalendarWidget>
#include <QCheckBox>

class OptionalCalendar: public QWidget
{
    Q_OBJECT

public:
    OptionalCalendar(QString title, QWidget* parent=NULL);
    void setSelectedDate(QDate date=QDate());
    QDate selectedDate();
private:
    QCalendarWidget calendar;
    QCheckBox checkbox;
};

#endif //GUI_OPTIONALCALENDAR_H
