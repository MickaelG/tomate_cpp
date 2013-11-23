
#include "gui_widgets.h"

void center_text(QGraphicsSimpleTextItem* text, QRectF rect)
{
    float text_width = text->boundingRect().width();
    float text_height = text->boundingRect().height();
    float rect_center_x = rect.center().x();
    float rect_center_y = rect.center().y();
    float text_x = rect_center_x - text_width / 2;
    float text_y = rect_center_y - text_height / 2;
    text->setPos(text_x, text_y);
}

QString toQString(std::string const &s)
{
    return QString::fromUtf8(s.c_str());
}

std::string fromQString(QString const &s)
{
    return std::string(s.toUtf8().data());
}

QDate toQDate(bg::date date)
{
    return QDate(date.year(), date.month(), date.day());
}

bg::date fromQDate(QDate date)
{
    if (date.month() == 0)
    {
        return bg::date(bg::not_a_date_time);
    }
    else
    { 
        return bg::date(date.year(), date.month(), date.day());
    }
}