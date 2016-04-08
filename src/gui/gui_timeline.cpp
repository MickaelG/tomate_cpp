
#include "log.h"
#include "gui_timeline.h"
#include "gui_utils.h"
#include "gui_controller.h"

#include "dataset.h"
#include "plot.h"

#include "partitionset.h"

#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsProxyWidget>
#include <QGridLayout>
#include <QColor>
#include <QPushButton>
#include <QSignalMapper>

int Margin = 20;
int SubMargin = 5;
int PixPerDay = 3;
int SquareUnit = 30;
int PlotHeight = 200;


int date_to_pos(QDate date, QDate date0)
{
    //Returns a pixel position from a date
    int from_date0 = date0.daysTo(date);
    int x = PixPerDay * from_date0;
    return x;
}

QDate pos_to_date(int pos, QDate date0)
{
    //Returns a pixel position from a date
    int nb_days = pos / PixPerDay;
    return date0.addDays(nb_days);
}

CropTimeRepresentation::CropTimeRepresentation(Crop &crop,
                                               float ypos,
                                               float height,
                                               QDate date0,
                                               QWidget* parent) :
    date0(date0), crop(crop), _global_rect(nullptr)
{
    QString text = toQString(crop.get_plant().get_name());

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
       Log::Warning("No rect as been drawn (" + to_string(ypos) + "/" +
                    to_string(height) + ")");
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

    int x0 = date_to_pos(start_date, this->date0);
    int x1 = date_to_pos(end_date, this->date0);
    int width = x1 - x0;

    QGraphicsRectItem* rect = new QGraphicsRectItem(x0, PlotHeight * ypos, width, PlotHeight * height);
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


MonthsRepresentation::MonthsRepresentation(QDate date_start, QDate date_end, QWidget* parent)
{
    //months on the top
    int first_year = date_start.year();
    int last_year = date_end.year();
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
            int x0 = date_to_pos(firstday, date_start);
            int x1 = date_to_pos(lastday, date_start);
            QGraphicsRectItem* rect = new QGraphicsRectItem(x0, - 30, x1 - x0, 20);
            rect->setParentItem(this);
            QGraphicsSimpleTextItem* text = new QGraphicsSimpleTextItem(firstday.toString("MMMM"));
            center_text(text, rect->rect());
            text->setParentItem(this);
        }
    }
}

class DateLineGraphicsItem : public QGraphicsItem
{
public:
    DateLineGraphicsItem(int length) : _length(length)
    {
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


TimeScene::TimeScene(DatasetModel& dataset_model,
                     CropSelectionController& selection_controller,
                     DateController& date_controller,
                     QWidget* parent) :
    QGraphicsScene(parent),
    dataset_model(dataset_model),
    selected_crop_repr(nullptr),
    _selection_controller(selection_controller),
    date_controller(date_controller)
{
    QObject::connect(&_selection_controller, SIGNAL(selection_changed(Crop*)), this, SLOT(selectCrop(Crop*)));
    QObject::connect(this, SIGNAL(crop_selected(Crop*)), &_selection_controller, SLOT(select_crop(Crop*)));

    QObject::connect(&date_controller, SIGNAL(date_changed(bool)), this, SLOT(date_changed(bool)));

    QObject::connect(&dataset_model, SIGNAL(updated()), this, SLOT(redraw()));

    draw_scene();
}

CropTimeRepresentation* TimeScene::getCropReprAtPos(QPointF scene_pos)
{
    CropTimeRepresentation* p_current_crop_repr = 0;
    for (CropTimeRepresentation* crop_repr: crop_reprs)
    {
        QRectF local_crop_rect = crop_repr->boundingRect();
        QRectF crop_rect = crop_repr->sceneTransform().mapRect(local_crop_rect);

        if (crop_rect.contains(scene_pos))
        {
            p_current_crop_repr = crop_repr;
        }
    }
    return p_current_crop_repr;
}

void TimeScene::removeCropSelection()
{
    if (selected_crop_repr != nullptr)
    {
        selected_crop_repr->set_selected(false);
    }
    selected_crop_repr = nullptr;
}

void TimeScene::selectCrop(Crop* p_crop)
{
    removeCropSelection();
    if (p_crop == nullptr) {
        return;
    }
    for (CropTimeRepresentation* crop_repr: crop_reprs)
    {
        if (crop_repr->get_pcrop() == p_crop)
        {
            selected_crop_repr = crop_repr;
            break;
        }
    }
    if (selected_crop_repr == nullptr) {
        return;
    }
    selected_crop_repr->set_selected(true);
}

void TimeScene::selectNextCrop(bool reverse)
{
    CropTimeRepresentation* crop_repr_to_select;
    if (selected_crop_repr == nullptr)
    {
        if (crop_reprs.size() > 0)
        {
          if (reverse)
          {
              crop_repr_to_select = crop_reprs.back();
          }
          else
          {
              crop_repr_to_select = crop_reprs.front();
          }
        }
    }
    else
    {
        int curr_index = find(crop_reprs.begin(), crop_reprs.end(), selected_crop_repr) - crop_reprs.begin();
        int next_index;
        if (reverse)
        {
            next_index = curr_index - 1;
        }
        else
        {
            next_index = curr_index + 1;
        }
        if (next_index < crop_reprs.size() && next_index >= 0)
        {
            crop_repr_to_select = crop_reprs[next_index];
        }
        else if (crop_reprs.size() > 0)
        {
            if (reverse)
            {
                crop_repr_to_select = crop_reprs.back();
            }
            else
            {
                crop_repr_to_select = crop_reprs.front();
            }
        }
    }
    emit crop_selected(crop_repr_to_select->get_pcrop());
}

void TimeScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF clic_point = event->scenePos();
    if ((clic_point.y() > -30) && (clic_point.y() < 0))
    {
        int xpos = clic_point.x();
        QDate date = pos_to_date(xpos, QDate(date_controller.get_date().year(), 1, 1));
        date_controller.set_date(date);
    }
    else {
        CropTimeRepresentation* p_current_crop_repr = getCropReprAtPos(clic_point);
        emit crop_selected(p_current_crop_repr ? p_current_crop_repr->get_pcrop() : nullptr);
    }
    QGraphicsScene::mousePressEvent(event);
}

void TimeScene::keyPressEvent(QKeyEvent* keyEvent)
{
    if (keyEvent->key() == Qt::Key_Tab)
    {
        selectNextCrop();
    }
    else if (keyEvent->key() == Qt::Key_Backtab)
    {
        selectNextCrop(true);
    }
}

void TimeScene::next_year() {
    QDate date = date_controller.get_date().addYears(1);
    date_controller.set_date(date);
}


void TimeScene::previous_year() {
    QDate date = date_controller.get_date().addYears(-1);
    date_controller.set_date(date);
}

void TimeScene::date_changed(bool year_changed) {
    if (year_changed) {
        redraw();
    } else {
        const QDate& date = date_controller.get_date();
        int xpos = date_to_pos(date, QDate(date.year(), 1, 1));
        _date_line->setPos(xpos, 0);
    }
}

void TimeScene::redraw()
{
    clear_scene();
    draw_scene();
}

void TimeScene::clear_scene()
{
    selected_crop_repr = 0;
    crop_reprs.clear();
    clear();
}


void TimeScene::add_year_buttons()
{
    const QDate& date = date_controller.get_date();

    QDate date0 = QDate(date.year(), 1, 1);
    QDate date_end = date0.addYears(1);

    int x0 = date_to_pos(date0, date0);
    int x1 = date_to_pos(date_end, date0);

    QPushButton *previous_year_button = new QPushButton("<<");
    previous_year_button->setGeometry(0, -54, 80, 24);
    QObject::connect(previous_year_button, SIGNAL(clicked(bool)), this, SLOT(previous_year()), Qt::QueuedConnection);
    QGraphicsProxyWidget* proxy_prev = addWidget(previous_year_button);

    QPushButton *next_year_button = new QPushButton(">>");
    next_year_button->setGeometry(x1-80, -54, 80, 24);
    QObject::connect(next_year_button, SIGNAL(clicked(bool)), this, SLOT(next_year()), Qt::QueuedConnection);
    QGraphicsProxyWidget* proxy_next = addWidget(next_year_button);

    QGraphicsRectItem* rect = new QGraphicsRectItem(x0, -54, x1 - x0, 24);
    addItem(rect);
    QGraphicsSimpleTextItem* text = new QGraphicsSimpleTextItem(date0.toString("yyyy"));
    center_text(text, rect->rect());
    addItem(text);
}

pair<float, float> compute_crop_ypos(const Crop& crop, const Plot& plot)
{
    float pos = 0;
    float height = crop.get_shape()->get_area() / plot.get_shape()->get_area();

    return make_pair(pos, height);
}


void TimeScene::draw_scene()
{
    //date0 is the 1 january of the first year if (a crop exists,
    //else 1 january of current year
    QDate date0 = QDate(date_controller.get_date().year(), 1, 1);
    MonthsRepresentation *months = new MonthsRepresentation(date0, date0.addYears(1).addDays(-1));

    addItem(months);
    add_year_buttons();

    Crops& crops = dataset_model.get_dataset().get_crops();

    qreal plot_max_pos = 0;
    for (const Plot& plot: dataset_model.get_dataset().get_plots())
    {
        qreal y_pos_start = plot_max_pos;
        if (y_pos_start != 0) {
            y_pos_start += Margin;
        }
        plot_max_pos += PlotHeight;
        //actual timeline
        list<Crop*> current_crops;
        vector<Rectangle> crop_rects;

        //Select crops in current year and current plot
        for (Crop& crop: crops)
        {
            if (crop.get_plot().get_key() == plot.get_key() and crop.is_in_year_started_by(fromQDate(date0)))
            {
                current_crops.push_back(&crop);
                crop_rects.push_back(*crop.get_shape());
            }
        }

        _2dTo1dConverter converter(*plot.get_shape(), crop_rects);
        for (Crop* crop_p: current_crops)
        {
            pair<float, float> ycoords = converter.get_position(*crop_p->get_shape());
            float ypos = ycoords.first;
            CropTimeRepresentation* crop_repr = new CropTimeRepresentation(*crop_p, (ypos == -1) ? 0 : ypos,
                                                                           ycoords.second, date0);
            crop_repr->setY(y_pos_start);

            if (ycoords.first == -1) {

                QList<QGraphicsItem*> colliding_items;
                do {
                    colliding_items = collidingItems(crop_repr);
                    qreal lowest_y = crop_repr->pos().y();
                    for (auto item: colliding_items) {
                        qreal low_y = item->pos().y() + item->boundingRect().height();
                        if (low_y > lowest_y) {
                            lowest_y = low_y;
                        }
                    }
                    crop_repr->setY(lowest_y+1);
                } while (!colliding_items.empty());
            }

            plot_max_pos = qMax(plot_max_pos, crop_repr->pos().y() + crop_repr->boundingRect().height());

            addItem(crop_repr);
            crop_reprs.push_back(crop_repr);
        }

        //Add plot name on the left
        QGraphicsSimpleTextItem* T = new QGraphicsSimpleTextItem(toQString(plot.get_name()));
        float text_width = T->boundingRect().width();
        T->setPos(-40, (y_pos_start + plot_max_pos + text_width) / 2);
        T->setRotation(-90);
        addItem(T);
    }
    _date_line = new DateLineGraphicsItem(plot_max_pos);
    int xpos = date_to_pos(date_controller.get_date(), QDate(date_controller.get_date().year(), 1, 1));
    _date_line->setPos(xpos, 0);
    this->addItem(_date_line);
    selectCrop(_selection_controller.get_selected());
    update();
    emit size_changed();
}
