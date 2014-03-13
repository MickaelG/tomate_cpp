
#ifndef GUI_OPTIONALCALENDAR_H
#define GUI_OPTIONALCALENDAR_H

#include <QCalendarWidget>
//#include <QDateEdit>
#include <QCheckBox>

class OptionalCalendar: public QWidget
{
    Q_OBJECT

public:
    OptionalCalendar(QString title, QWidget* parent=NULL);
    void setSelectedDate(QDate date=QDate());
    QDate selectedDate();
    void setEnabled(bool enabled=true);

private slots:
    void updateState();
private:
    QCalendarWidget calendar;
    //QDateEdit calendar;
    QCheckBox checkbox;
};

#endif //GUI_OPTIONALCALENDAR_H
