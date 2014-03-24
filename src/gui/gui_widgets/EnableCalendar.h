#ifndef ENABLECALENDAR_H
#define ENABLECALENDAR_H

#include <QWidget>
#include <QDate>

namespace Ui {
class EnableCalendar;
}

class EnableCalendar : public QWidget
{
    Q_OBJECT

public:
    explicit EnableCalendar(QWidget *parent = 0);
    ~EnableCalendar();
    void setSelectedDate(QDate date=QDate());
    QDate selectedDate();
    void setEnabled(bool enabled=true);

signals:
    void dateChanged(QDate);

private slots:
    void updateState();

private:
    Ui::EnableCalendar *ui;
};

#endif // ENABLECALENDAR_H
