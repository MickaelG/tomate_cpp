
#include <QGraphicsItemGroup>
#include <QDate>


class DateController;


class DateRuler: public QGraphicsItem
{
public:
    DateRuler(DateController& date_controller,
              int line_length);

    
    void update_date();

    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    static int date_to_pos(QDate date, QDate date0);
    static QDate pos_to_date(int pos, QDate date0);
    
private:
    DateController& _date_controller;
    QGraphicsItem* _date_line;
};

