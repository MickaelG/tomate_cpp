
#ifndef GUI_CROPTIMEREPRESENTION_H
#define GUI_CROPTIMEREPRESENTION_H

#include <QDate>
#include <QGraphicsItemGroup>


class Crop;

class CropTimeRepresentation: public QGraphicsItemGroup
{
public:
    CropTimeRepresentation(Crop &crop, float ypos, float height,
                           QDate date0 = QDate(), QWidget* parent = NULL);
    QGraphicsRectItem* create_rect(QDate start_date, QDate end_date,
                                   float ypos, float height, bool planned=false);
    void delete_me();
    const Crop *get_pcrop() const;
    Crop *get_pcrop();
    //void show(QPoint point, Crop* p_crop);
    void set_selected(bool val);
    void _date_ruler(QDate end_date, QDate date0);

private:
    void update_global_rect(const QGraphicsRectItem& in_rect);

    Crop& crop;
    QDate date0;
    QGraphicsRectItem* _global_rect;
};



#endif //GUI_CROPTIMEREPRESENTATION_H
