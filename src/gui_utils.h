
#ifndef GUI_UTILS_H
#define GUI_UTILS_H

#include <QGraphicsSimpleTextItem>
void center_text(QGraphicsSimpleTextItem* text, QRectF rect);

#include <QString>
#include <string>
QString toQString(std::string const &s);
std::string fromQString(QString const &s);

#include <boost/date_time.hpp>
#include <QDate>
namespace bg = boost::gregorian;
QDate toQDate(bg::date date);
bg::date fromQDate(QDate date);

#endif //GUI_UTILS_H
