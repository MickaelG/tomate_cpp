
#ifndef GUI_CALENDAR_H
#define GUI_CALENDAR_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenu>
#include <QDate>
#include <QGraphicsItemGroup>

#include <vector>
using namespace std;

class Calendar;
class CalendarEvent;


class CalendarScene: public QGraphicsScene
{
  Q_OBJECT

public:
    CalendarScene(const Calendar& calendar);
    void draw();

private:
    void AddEvent(const CalendarEvent& event);
    void AddEvent(QDate start_date, QDate end_date, QString title);
    void AddEvent(QDate start_date, QString title);
    qreal GetYCoord(QDate date);

    const Calendar& _calendar;
};

#endif //GUI_CALENDAR_H
