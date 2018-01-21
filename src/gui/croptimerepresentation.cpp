
#include <QPen>
#include <iostream>
#include <QGraphicsScene>

#include "log.h"
#include "croptimerepresentation.h"
#include "DateRuler.h"
#include "crops.h"
#include "plant.h"
#include "gui_utils.h"


CropTimeRepresentation::CropTimeRepresentation(Crop &crop,
                                               float ypos,
                                               float height,
                                               QDate date0,
                                               QWidget* parent) :
    crop(crop), date0(date0), _global_rect(nullptr)
{
    QString text = toQString(crop.get_plant().get_species().get_name());

    QDate p_start_date = toQDate(crop.get_virtual_planned_start_date());
    QDate p_end_date = toQDate(crop.get_virtual_planned_end_date());
    if (p_start_date > p_end_date) {
        std::cout << "Error with crop " << crop.get_plant().get_name() << std::endl;
    }
    QDate start_date = toQDate(crop.get_date(Crop::DateSel::START));
    QDate end_date = toQDate(crop.get_virtual_end_date());

    QGraphicsItemGroup* curr_group = new QGraphicsItemGroup();
    QGraphicsRectItem* planned_rect = create_rect(p_start_date, p_end_date, ypos, height, true);
    if (planned_rect != nullptr) {
        update_global_rect(*planned_rect);
        curr_group->addToGroup(planned_rect);
    }
    QGraphicsRectItem* real_rect = create_rect(start_date, end_date, ypos, height, false);
    if (real_rect != nullptr) {
       update_global_rect(*real_rect);
       curr_group->addToGroup(real_rect);
    }

    if (planned_rect == nullptr && real_rect == nullptr) {
       Log::Warning("No rect as been drawn (" + std::to_string(ypos) + "/" +
                    std::to_string(height) + ")");
    }

    QGraphicsSimpleTextItem* textw = new QGraphicsSimpleTextItem(text);
    center_text(textw, curr_group->boundingRect());
    textw->setParentItem(curr_group);
    curr_group->addToGroup(new QGraphicsRectItem(boundingRect()));
    this->addToGroup(curr_group);

    if (_global_rect != nullptr) {
        this->addToGroup(_global_rect);
    }
}

void CropTimeRepresentation::update_global_rect(const QGraphicsRectItem& in_rect)
{
    if (_global_rect == nullptr) {
        _global_rect = new QGraphicsRectItem(in_rect.rect());
        return;
    }

    int in_x1 = in_rect.rect().x();
    int in_y1 = in_rect.rect().y();
    int in_x2 = in_x1 + in_rect.rect().width();
    int in_y2 = in_y1 + in_rect.rect().height();

    int g_x1 = _global_rect->rect().x();
    int g_y1 = _global_rect->rect().y();
    int g_x2 = g_x1 + _global_rect->rect().width();
    int g_y2 = g_y1 + _global_rect->rect().height();

    QRectF new_rect(_global_rect->rect());

    if (in_x1 < g_x1) {
        new_rect.setX(in_x1);
    }
    if (in_y1 < g_y1) {
        new_rect.setY(in_y1);
    }
    if (in_x2 > g_x2) {
        new_rect.setWidth(in_x2 - _global_rect->rect().x());
    }
    if (in_y2 > g_y2) {
        new_rect.setHeight(in_y2 - _global_rect->rect().y());
    }
    _global_rect->setRect(new_rect);

}


QGraphicsRectItem* CropTimeRepresentation::create_rect(QDate start_date, QDate end_date, float ypos, float height, bool planned)
{
    if (!(start_date.isValid() && end_date.isValid()))
    {
        return nullptr;
    }
    if (start_date > date0.addYears(1) || end_date < date0)
    {
        return nullptr;
    }
    if (start_date < date0) { start_date = date0.addDays(-3); }
    if (end_date > date0.addYears(1)) { end_date = date0.addYears(1).addDays(3); }

    int x0 = DateRuler::date_to_pos(start_date, this->date0);
    int x1 = DateRuler::date_to_pos(end_date, this->date0);
    int width = x1 - x0;

    QGraphicsRectItem* rect = new QGraphicsRectItem(x0, ypos, width, height);
    QString color_str = toQString(crop.get_plant().get_color_str());
    //TODO: handle default color in dataset
    if (color_str == "") { color_str = "#FF00FF"; }
    QColor color = QColor(color_str);
    if (planned)
    {
        rect->setBrush(QBrush(color, Qt::BDiagPattern));
    }
    else
    {
        rect->setBrush(QBrush(color));
    }
    rect->setPen(QPen(Qt::NoPen));
    
    return rect;
}


//void CropTimeRepresentation::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
//{
//    context_menu->show(event->screenPos(), &crop);
//    //QMenu menu;
//    //QAction *removeAction = menu.addAction("Remove");
//    //QAction *markAction = menu.addAction("Mark");
//    //QAction *selectedAction = menu.exec(event->screenPos());
//}


void CropTimeRepresentation::delete_me()
{
    this->scene()->removeItem(this);
}

const Crop* CropTimeRepresentation::get_pcrop() const
{
    return &crop;
}

Crop* CropTimeRepresentation::get_pcrop()
{
    return &crop;
}


void CropTimeRepresentation::set_selected(bool sel)
{
    if (_global_rect == nullptr) {
        std::cout << "Warning: global_rect is null" << std::endl;
        return;
    }

    if (sel) {
      QPen selected_pen;
      selected_pen.setWidth(4);
      //selected_pen.setColor(QColor("#444444"));
      _global_rect->setPen(selected_pen);
    } else {
      _global_rect->setPen(QPen());
    }
}

