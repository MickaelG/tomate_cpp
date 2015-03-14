
#include "gui_timeline.h"
#include "gui_utils.h"

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


CropTimeRepresentation::CropTimeRepresentation(Crop& crop, const list<pair<float, float> >& y_coords, QDate date0, QWidget* parent) :
    date0(date0), crop(crop)
{
    QString text = toQString(crop.get_plant().get_name());

    QDate p_start_date = toQDate(crop.get_virtual_planned_start_date());
    QDate p_end_date = toQDate(crop.get_virtual_planned_end_date());
    if (p_start_date > p_end_date) {
        std::cout << "Error with crop " << crop.get_plant().get_name() << std::endl;
    }
    QDate start_date = toQDate(crop.get_date("start"));
    QDate end_date = toQDate(crop.get_virtual_end_date());

    for (pair<float, float> yc: y_coords) {
        QGraphicsItemGroup* curr_group = new QGraphicsItemGroup();
        QGraphicsRectItem* planned_rect = create_rect(p_start_date, p_end_date, yc.first, yc.second, true);
        if (planned_rect) {
          curr_group->addToGroup(planned_rect);
        }
        QGraphicsRectItem* real_rect = create_rect(start_date, end_date, yc.first, yc.second, false);
        if (real_rect) {
          curr_group->addToGroup(real_rect);
        }

        QGraphicsSimpleTextItem* textw = new QGraphicsSimpleTextItem(text);
        center_text(textw, curr_group->boundingRect());
        textw->setParentItem(curr_group);
        curr_group->addToGroup(new QGraphicsRectItem(boundingRect()));
        this->addToGroup(curr_group);
    }
    if (_global_rect) {
        //delete _global_rect;
    }
    _global_rect = new QGraphicsRectItem(boundingRect());
    this->addToGroup(_global_rect);
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

Crop* CropTimeRepresentation::get_pcrop()
{
    return &crop;
}

void CropTimeRepresentation::set_selected(bool sel)
{
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


WholeTimeScene::WholeTimeScene(Dataset& dataset, QWidget* parent) :
    dataset(dataset), selected_crop(NULL), selected_crop_repr(NULL)
{
    //context_menu = new QMenu;
    //context_menu->addAction(tr("Edit"), edit_crop_dialog, SLOT(show()));
    date = QDate::currentDate();
    draw_scene();
}

CropTimeRepresentation* WholeTimeScene::getCropReprAtPos(QPointF scene_pos)
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

void WholeTimeScene::drawCropSelection()
{
    if (selected_crop && !selected_crop_repr)
    {
        for (CropTimeRepresentation* crop_repr: crop_reprs)
        {
            if (crop_repr->get_pcrop() == selected_crop)
            {
                selected_crop_repr = crop_repr;
                break;
            }
        }

    }
    if (selected_crop_repr)
    {
        selected_crop_repr->set_selected(true);
    }
    emit crop_selected(selected_crop);
}

void WholeTimeScene::removeCropSelection()
{
    if (selected_crop_repr)
    {
        selected_crop_repr->set_selected(false);
    }
}

void WholeTimeScene::selectCrop(Crop* p_crop)
{
    removeCropSelection();
    selected_crop_repr = 0;
    selected_crop = p_crop;
    drawCropSelection();
}

void WholeTimeScene::selectNextCrop(bool reverse)
{
    removeCropSelection();
    if (!selected_crop)
    {
        if (crop_reprs.size() > 0)
        {
          if (reverse)
          {
            selected_crop_repr = crop_reprs.back();
          }
          else
          {
            selected_crop_repr = crop_reprs[0];
          }
          selected_crop = selected_crop_repr->get_pcrop();
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
            selected_crop_repr = crop_reprs[next_index];
            selected_crop = selected_crop_repr->get_pcrop();
        }
        else if (crop_reprs.size() > 0)
        {
            if (reverse)
            {
                selected_crop_repr = crop_reprs.back();
            }
            else
            {
                selected_crop_repr = crop_reprs[0];
            }
            selected_crop = selected_crop_repr->get_pcrop();
        }
    }
    drawCropSelection();
}

void WholeTimeScene::selectCrop(CropTimeRepresentation* p_crop_repr)
{
    removeCropSelection();
    selected_crop_repr = p_crop_repr;
    if (p_crop_repr)
    {
        selected_crop = p_crop_repr->get_pcrop();
    }
    else
    {
        selected_crop = 0;
    }
    drawCropSelection();
}

/*
void WholeTimeScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    CropTimeRepresentation* p_current_crop_repr = getCropReprAtPos(event->scenePos());
    QAction* action = context_menu->actions().first();
    if (p_current_crop_repr)
    {
        edit_crop_dialog->set_crop_values(p_current_crop_repr->get_pcrop());
        action->setText(tr("Edit crop"));
    }
    else
    {
        edit_crop_dialog->set_crop_values(0);
        action->setText(tr("New crop"));
    }
    context_menu->exec(event->screenPos());
}
*/

void WholeTimeScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF clic_point = event->scenePos();
    if ((clic_point.y() > -30) && (clic_point.y() < 0))
    {
        int xpos = clic_point.x();
        date = pos_to_date(xpos, QDate(date.year(), 1, 1));
        current_date_changed(date);
        //redraw();
        _date_line->setPos(xpos, 0);
    }
    else {
        CropTimeRepresentation* p_current_crop_repr = getCropReprAtPos(clic_point);
        selectCrop(p_current_crop_repr);
    }
    QGraphicsScene::mousePressEvent(event);
}

void WholeTimeScene::keyPressEvent(QKeyEvent* keyEvent)
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

void WholeTimeScene::next_year() {
    date = date.addYears(1);
    current_date_changed(date);
    redraw();
}


void WholeTimeScene::previous_year() {
    date = date.addYears(-1);
    current_date_changed(date);
    redraw();
}


void WholeTimeScene::redraw()
{
    clear_scene();
    draw_scene();
}

void WholeTimeScene::clear_scene()
{
    selected_crop_repr = 0;
    crop_reprs.clear();
    clear();
}


void WholeTimeScene::add_year_buttons()
{
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


void WholeTimeScene::draw_scene()
{
    //date0 is the 1 january of the first year if (a crop exists,
    //else 1 january of current year
    QDate date0 = QDate(date.year(), 1, 1);
    MonthsRepresentation *months = new MonthsRepresentation(date0, date0.addYears(1).addDays(-1));

    addItem(months);
    add_year_buttons();

    Crops& crops = dataset.get_crops();

    int y_pos = 0;
    for (const Plot plot: dataset.get_plots())
    {
        int y_pos_start = y_pos;
        //actual timeline
        list<Crop*> current_crops;

        //Select crops in current year and current plot
        for (Crop& crop: crops)
        {
            if (crop.get_plot().get_key() == plot.get_key() and crop.is_in_year_started_by(fromQDate(date0)))
            {
                current_crops.push_back(&crop);
            }
        }

        list<Rectangle> partitions = compute_partitions(current_crops, plot);

        for (Crop* crop_p: current_crops)
        {
            list< pair<float, float> > ycoords = compute_timerepr(*crop_p, partitions);
            CropTimeRepresentation* crop_repr = new CropTimeRepresentation(*crop_p, ycoords, date0);
            crop_repr->setY(y_pos);
            addItem(crop_repr);
            crop_reprs.push_back(crop_repr);
        }
        y_pos += Margin + PlotHeight;

        //Add plot name on the left
        QGraphicsSimpleTextItem* T = new QGraphicsSimpleTextItem(toQString(plot.get_name()));
        float text_width = T->boundingRect().width();
        T->setPos(-40, (y_pos_start + y_pos + text_width) / 2);
        T->setRotation(-90);
        addItem(T);
    }
    _date_line = new DateLineGraphicsItem(y_pos);
    int xpos = date_to_pos(date, QDate(date.year(), 1, 1));
    _date_line->setPos(xpos, 0);
    this->addItem(_date_line);
    drawCropSelection();
    update();
}

WholeTimeSceneView::WholeTimeSceneView(Dataset& dataset, QWidget* parent)
{
    _scene = new WholeTimeScene(dataset, parent);
    this->setScene(_scene);
    this->update_rect();
}

void WholeTimeSceneView::update_draw()
{
    get_scene()->redraw();
    update_rect();
}

void WholeTimeSceneView::update_rect()
{
    QRectF itemsRect = scene()->itemsBoundingRect();
    int x = itemsRect.x();
    int y = itemsRect.y();
    int width = itemsRect.width();
    int height = itemsRect.height();
    this->setSceneRect(x - Margin, y - Margin,
                      width + 2 * Margin, height + 2 * Margin);
    this->setAlignment(Qt::AlignTop);
}

TimelineWindow::TimelineWindow(Dataset& dataset, QWidget* parent) :
    view(dataset, this)
{
    this->setLayout(new QGridLayout);
    this->layout()->addWidget(&view);
}


void TimelineWindow::update_draw()
{
    view.update_draw();
}
