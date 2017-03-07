
#include "DateRuler.h"

#include "gui_utils.h"
#include "gui_controller.h"

#include <QDate>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>


class DateLineGraphicsItem : public QGraphicsItem
{
public:
    DateLineGraphicsItem(QGraphicsItem* parent, int length) :
        QGraphicsItem(parent),
        _length(length)
    {
        setZValue(20);
    }

    QRectF boundingRect() const
    {
        return QRectF(-_point_size/2, -30, _point_size, 30+_length);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget)
    {
        QColor red("red");
        painter->setPen(QPen(red));
        painter->setBrush(QBrush(red));
        painter->drawLine(0, -30, 0, _length);
        painter->drawEllipse(-_point_size/2, -14, _point_size, _point_size);
    }
private:
    int _length;
    int _point_size = 8;
};

class TodayButtonGraphicsItem: public QGraphicsEllipseItem
{
public:
    
    TodayButtonGraphicsItem(qreal x, qreal y, qreal size,
                            DateController& date_controller,
                            QGraphicsItem* parent) :
        QGraphicsEllipseItem(x, y, size, size, parent),
        _date_controller(date_controller)
    {
        setBrush(QBrush(QColor(243, 255, 243, 240)));
        setPen(Qt::NoPen);
        setZValue(10);
    }
    
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event)
    {
        _date_controller.set_date(QDate::currentDate());
    }
    
private:
    DateController& _date_controller;
    int _year_incr;

};

class MonthsGraphicsItem : public QGraphicsItemGroup
{
  
public:
  
  MonthsGraphicsItem(QGraphicsItem* parent, DateController& date_controller) :
    QGraphicsItemGroup(parent),
    _date_controller(date_controller)
  {
    const unsigned height = 20;
    
    QDate date0 = QDate(_date_controller.get_date().year(), 1, 1);
    int first_year = date_controller.get_date().year();

    QDate date_start = QDate(first_year, 1, 1);
  
    int last_year = first_year;
    for (int year=first_year; year<=last_year; year++)
    {
        for (int month=1; month<=12; month++)
        {
            QDate firstday(year, month, 1);
            QDate lastday;
            if (month == 12)
            {
                lastday = QDate(year + 1, 1, 1);
            }
            else
            {
                lastday = QDate(year, month + 1, 1);
            }
            int x0 = DateRuler::date_to_pos(firstday, date_start);
            int x1 = DateRuler::date_to_pos(lastday, date_start);
            QGraphicsRectItem* rect = new QGraphicsRectItem(x0, - 30, x1 - x0, height);
            rect->setParentItem(this);
            QGraphicsSimpleTextItem* text = new QGraphicsSimpleTextItem(firstday.toString("MMMM"));
            center_text(text, rect->rect());
            text->setParentItem(this);
        }
    }
  }
  
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override
  {
      QPointF clic_point = event->scenePos();
      int xpos = clic_point.x();
      QDate date = DateRuler::pos_to_date(xpos, QDate(_date_controller.get_date().year(), 1, 1));
      _date_controller.set_date(date);
  }

  void wheelEvent(QGraphicsSceneWheelEvent *event) override
  {
      _date_controller.incr_days(event->delta() / 120);
  }
  
private:
  DateController& _date_controller;

};

class YearButtonGraphicsItem: public QGraphicsEllipseItem
{
public:
    
    YearButtonGraphicsItem(qreal x, qreal y, qreal size,
                           DateController& date_controller,
                           int year_incr,
                           QGraphicsItem* parent) :
        QGraphicsEllipseItem(x, y, size, size, parent),
        _date_controller(date_controller),
        _year_incr(year_incr)
    {
        setBrush(QBrush(QColor(243, 255, 243)));
        setPen(Qt::NoPen);
        auto* title = new QGraphicsSimpleTextItem((year_incr < 0) ? "-" : "+", this);
        center_text(title, boundingRect());
    }
    
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event)
    {
        _date_controller.incr_years(_year_incr);        
    }
    
private:
    DateController& _date_controller;
    int _year_incr;

};

class YearsGraphicsItem : public QGraphicsRectItem
{
  
public:
  
    YearsGraphicsItem(QGraphicsItem* parent, DateController& date_controller) :
      QGraphicsRectItem(parent),
      _date_controller(date_controller)
    {
        const unsigned height = 24;
        
        const QDate& date = _date_controller.get_date();

        QDate date0 = QDate(date.year(), 1, 1);
        QDate date_end = date0.addYears(1);

        int x0 = DateRuler::date_to_pos(date0, date0);
        int x1 = DateRuler::date_to_pos(date_end, date0);

        auto* prev_year_button = new YearButtonGraphicsItem(1, -54, height, _date_controller, -1, this);
        auto* next_year_button = new YearButtonGraphicsItem(x1-24-1, -54, height, _date_controller, 1, this);

        setRect(x0, -54, x1 - x0, height);
        setZValue(10);
        QGraphicsSimpleTextItem* text = new QGraphicsSimpleTextItem(date0.toString("yyyy"), this);
        center_text(text, rect());
    }

    void wheelEvent(QGraphicsSceneWheelEvent *event)
    {
        _date_controller.incr_years(event->delta() / 120);
    }
    
private:
    DateController& _date_controller;
};

DateRuler::DateRuler(DateController& date_controller,
                     int line_length):
    _date_controller(date_controller)
{
    new YearsGraphicsItem(this, date_controller);
    new MonthsGraphicsItem(this, date_controller);
    
    QDate date_start(_date_controller.get_date().year(), 1, 1);
    _date_line = new DateLineGraphicsItem(this, line_length);
    int xpos = date_to_pos(_date_controller.get_date(), date_start);
    _date_line->setPos(xpos, 0);
    
    QDate today = QDate::currentDate();
    if (today.year() == date_start.year()) {
        const unsigned height = 15;
        qreal button_xpos = DateRuler::date_to_pos(today, date_start) - height/2;
        new TodayButtonGraphicsItem(button_xpos, -20, height, _date_controller, this);
    }
}

void DateRuler::update_date()
{
    const QDate& date = _date_controller.get_date();
    int xpos = date_to_pos(date, QDate(date.year(), 1, 1));
    _date_line->setPos(xpos, 0);
}

QRectF DateRuler::boundingRect() const
{
    return QRectF();
}

void DateRuler::paint(QPainter *painter,
                      const QStyleOptionGraphicsItem *option,
                      QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}
  
const int PixPerDay = 3;

int DateRuler::date_to_pos(QDate date, QDate date0)
{
    //Returns a pixel position from a date
    int from_date0 = date0.daysTo(date);
    int x = PixPerDay * from_date0;
    return x;
}

QDate DateRuler::pos_to_date(int pos, QDate date0)
{
    //Returns a pixel position from a date
    int nb_days = pos / PixPerDay;
    return date0.addDays(nb_days);
}
