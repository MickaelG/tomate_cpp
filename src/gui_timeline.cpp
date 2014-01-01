
#include "gui_timeline.h"
#include "gui_utils.h"

#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsProxyWidget>
#include <QGridLayout>
#include <stdexcept>
#include <QColor>
#include <QSignalMapper>

int Margin = 20;
int SubMargin = 5;
int PixPerDay = 3;
int SquareUnit = 30;


int date_to_pos(QDate date, QDate date0)
{
    //Returns a pixel position from a date
    int from_date0 = date0.daysTo(date);
    int x = PixPerDay * from_date0;
    return x;
}


CropTimeRepresentation::CropTimeRepresentation(Crop& crop, QDate date0, QWidget* parent) :
    date0(date0), crop(crop)
{
    QString text = toQString(crop.get_plant().get_name());

    QDate start_date = toQDate(crop.get_date("planned_start"));
    QDate end_date = toQDate(crop.get_virtual_planned_end_date());
    if (start_date > end_date) {
        std::cout << "Error with crop " << crop.get_plant().get_name() << std::endl;
    }
    QGraphicsRectItem* acrect = create_rect(start_date, end_date, true);
    if (acrect) {
        this->addToGroup(acrect);
    }
    
    start_date = toQDate(crop.get_date("start"));
    end_date = toQDate(crop.get_virtual_end_date());
    QGraphicsRectItem* plrect = create_rect(start_date, end_date, false);
    if (plrect)
    {
        this->addToGroup(plrect);
    }

    QGraphicsSimpleTextItem* textw = new QGraphicsSimpleTextItem(text);
    center_text(textw, this->boundingRect());
    textw->setParentItem(this);
    QGraphicsRectItem* rect = new QGraphicsRectItem(boundingRect());
    this->addToGroup(rect);
}


QGraphicsRectItem* CropTimeRepresentation::create_rect(QDate start_date, QDate end_date, bool planned)
{
    if (!(start_date.isValid() && end_date.isValid()))
    {
        return NULL;
    }
    if (start_date > date0.addYears(1) || end_date < date0)
    {
        return NULL;
    }
    if (start_date < date0) { start_date = date0.addDays(-3); }
    if (end_date > date0.addYears(1)) { end_date = date0.addYears(1).addDays(3); }

    int x0 = date_to_pos(start_date, this->date0);
    int x1 = date_to_pos(end_date, this->date0);
    int width = x1 - x0;

    int y = 0;
    int height = SquareUnit;
    QGraphicsRectItem* rect = new QGraphicsRectItem(x0, y, width, height);
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


WholeTimeScene::WholeTimeScene(Dataset& dataset, QWidget* parent) :
    dataset(dataset)
{
    edit_crop_dialog = new EditCropDialog(dataset);
    context_menu = new QMenu;
    context_menu->addAction(tr("Edit"), edit_crop_dialog, SLOT(show()));
    year = QDate::currentDate().year();
    draw_scene();
}

void WholeTimeScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    Crop* p_current_crop = 0;
    QGraphicsItem* itempos = itemAt(event->scenePos());
    QPointF clic_point = event->scenePos();
    for (CropTimeRepresentation* crop_repr: crop_reprs)
    {
        QRectF local_crop_rect = crop_repr->boundingRect();
        QRectF crop_rect = crop_repr->sceneTransform().mapRect(local_crop_rect);

        if (crop_rect.contains(clic_point))
        {
            p_current_crop = crop_repr->get_pcrop();
        }
    }
    edit_crop_dialog->set_crop_values(p_current_crop);
    QAction* action = context_menu->actions().first();
    if (p_current_crop)
    {
        action->setText(tr("Edit crop"));
    }
    else
    {
        action->setText(tr("New crop"));
    }
    context_menu->exec(event->screenPos());
}

void WholeTimeScene::next_year() {
    year++;
    redraw();
}


void WholeTimeScene::previous_year() {
    year--;
    redraw();
}


void WholeTimeScene::redraw()
{
    clear();
    draw_scene();
}


void WholeTimeScene::add_year_buttons()
{
    QDate date0 = QDate(year, 1, 1);
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
    int y_pos = 0;
    //date0 is the 1 january of the first year if (a crop exists,
    //else 1 january of current year
    Crops& crops = dataset.get_crops();

    add_year_buttons();
    crop_reprs.clear();
    
    QDate date0 = QDate(year, 1, 1);
    MonthsRepresentation *months = new MonthsRepresentation(date0, date0.addYears(1).addDays(-1));
    addItem(months);
    for (const Plot plot: dataset.get_plots())
    {
        for (Plot square: plot.get_subplots())
        {
            //subdiv numbers on the left
            QGraphicsSimpleTextItem* T = new QGraphicsSimpleTextItem(toQString(square.get_key()));
            T->setPos(-40, y_pos + 8);
            addItem(T);

            //actual timeline
            for (Crop& crop: crops)
            {
                if (crop.get_plot().get_key() == square.get_key() and crop.is_in_year_started_by(fromQDate(date0)))
                {
                    CropTimeRepresentation* crop_repr = new CropTimeRepresentation(crop, date0);
                    crop_repr->setY(y_pos);
                    addItem(crop_repr);
                    crop_reprs.push_back(crop_repr);
                }
            }
            y_pos += SquareUnit + SubMargin;
        }
        y_pos += Margin;
    }
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
    int x = sceneRect().x();
    int y = sceneRect().y();
    int width = sceneRect().width();
    int height = sceneRect().height();
    this->setSceneRect(x - Margin, y - Margin,
                      width + 2 * Margin, height + 2 * Margin);
}


TimelineWindow::TimelineWindow(Dataset& dataset, QWidget* parent) :
    view(dataset, this)
{
    this->setLayout(new QGridLayout);
    this->layout()->addWidget(&view);
}


void TimelineWindow::update()
{
    view.update_draw();
}
