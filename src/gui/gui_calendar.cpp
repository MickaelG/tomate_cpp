
#include "gui_calendar.h"
#include "gui_utils.h"

#include "calendar.h"

CalendarScene::CalendarScene(const Calendar& calendar) : _calendar(calendar)
{
    draw();
}

void CalendarScene::draw()
{
    for (const auto& event: _calendar.GetEvents(bg::date(2014,5,1), bg::date(2014,5,31))) {
        AddEvent(*event);
    }
}

void CalendarScene::AddEvent(const CalendarEvent& event)
{
    bg::date start_date = event.GetStartDate();
    bg::date end_date = event.GetEndDate();
    if (start_date.is_not_a_date()) {
        return;
    }
    if (end_date.is_not_a_date()) {
        return;
    }
    //AddEvent(toQDate(start_date), toQDate(end_date),
    //         toQString(event.GetTitle()));
    AddEvent(toQDate(start_date),
             toQString(event.GetTitle()));
}

void CalendarScene::AddEvent(QDate start_date, QDate end_date, QString title)
{
    qreal y_start = GetYCoord(start_date);
    qreal y_end = GetYCoord(end_date);
    addLine(10, y_start, 20, y_end);

    QGraphicsTextItem * text = new QGraphicsTextItem(title);
    qreal text_height = text->boundingRect().height();
    qreal y_text = y_start - text_height / 2;
    text->setPos(30, y_text);
    QList<QGraphicsItem*> colliding_items;
    do {
        colliding_items = collidingItems(text);
        qreal rightmost_x = text->pos().x();
        for (auto item: colliding_items) {
            qreal right_x = item->pos().x() + item->boundingRect().width();
            if (right_x > rightmost_x) {
                rightmost_x = right_x;
            }
        }
        text->setPos(rightmost_x + 1, y_text);

    } while (!colliding_items.empty());
    addItem(text);
}

void CalendarScene::AddEvent(QDate start_date, QString title)
{
    qreal y_pos = GetYCoord(start_date);
    addLine(10, y_pos, 20, y_pos);

    QGraphicsTextItem * text = new QGraphicsTextItem(title);
    qreal text_height = text->boundingRect().height();
    qreal y_text = y_pos - text_height / 2;
    text->setPos(30, y_text);
    QList<QGraphicsItem*> colliding_items;
    do {
        colliding_items = collidingItems(text);
        qreal rightmost_x = text->pos().x();
        for (auto item: colliding_items) {
            qreal right_x = item->pos().x() + item->boundingRect().width();
            if (right_x > rightmost_x) {
                rightmost_x = right_x;
            }
        }
        text->setPos(rightmost_x + 1, y_text);

    } while (!colliding_items.empty());
    addItem(text);
}

qreal CalendarScene::GetYCoord(QDate date)
{
    if (!date.isValid()) {
        throw logic_error("Invalid date");
    }
    qreal PixPerDay = 15;
    QDate date0(2013, 1, 1);
    int from_date0 = date0.daysTo(date);
    qreal y = PixPerDay * from_date0;
    return y;
}
