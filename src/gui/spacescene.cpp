
#include "spacescene.h"
#include "gui_controller.h"
#include "DateRuler.h"

#include "gui_utils.h"

#include "dataset.h"
#include "plot.h"

#include <QLayout>
#include <QFont>
#include <QColor>
#include <QBrush>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSimpleTextItem>
                

PlotRepresentation::PlotRepresentation(Crops& crops, Plot& plot, QDate date) :
    crops(crops), plot(plot)
{
    QPen pen;
    pen.setWidth(0);
    setPen(pen);

    this->update_draw(date);
}

void PlotRepresentation::update_draw(QDate date)
{
    vector<Crop*> crops_to_plot = crops.find_crops(plot, fromQDate(date));
    update_draw(crops_to_plot, date);
}

void PlotRepresentation::update_draw(const vector<Crop*>& crops_to_plot, QDate date)
{
    const Shape& shape = plot.get_shape();
    this->setRect(shape.get_min_x(), - shape.get_min_y() - shape.get_height(),
                  shape.get_width(), shape.get_height());

    add_name();

    for (Crop* p_crop: crops_to_plot)
    {
        CropSpaceRepr *crop_repr = new CropSpaceRepr(p_crop, date);
        crop_repr->setParentItem(this);
        crop_reprs.push_back(crop_repr);
    }
}
//TODO: delete subd_repr

void PlotRepresentation::add_name()
{
    QGraphicsSimpleTextItem* plot_title_item = new QGraphicsSimpleTextItem(toQString(plot.get_name()), this);
    QFont font = plot_title_item->font();
    font.setPointSize(int(this->rect().width()/20));
    font.setWeight(25);
    plot_title_item->setFont(font);

    float text_height = plot_title_item->boundingRect().height();
    plot_title_item->setPos(this->rect().left(), this->rect().top() - text_height);
}
            
CropSpaceRepr::CropSpaceRepr(Crop* p_crop, QDate date) :
    p_crop(p_crop)
{
    set_selected(false);

    Shape& shape = p_crop->get_shape();
    if (!shape)
    {
        return;
    }

    //TODO draw the real shape instead of a rect
    setRect(QRectF(shape.get_min_x(), - shape.get_min_y() - shape.get_height(),
                   shape.get_width(), shape.get_height()));
    if (p_crop)
    {
        Plant& plant = p_crop->get_plant();
        QString text = toQString(plant.get_species().get_name());
        //text = re.sub("\s+", "\n", text)
        QGraphicsSimpleTextItem *textw =  new QGraphicsSimpleTextItem(text);
        QFont font = textw->font();
        font.setPointSize(int(this->boundingRect().width()/10));
        font.setWeight(25);
        textw->setFont(font);
        center_text(textw, this->boundingRect());
        textw->setParentItem(this);
        QString color_str = toQString(plant.get_color_str());
        if (color_str == "") { color_str = "#FF00FF"; }
        QColor color = QColor(color_str);
        if (p_crop->is_planned_at_date(fromQDate(date)) && !p_crop->is_active_at_date(fromQDate(date)))
        {
            brush = QBrush(color, Qt::BDiagPattern);
        }
        else
        {
            brush = QBrush(color);
        }
    }

}

Crop* CropSpaceRepr::get_pcrop()
{
    return p_crop;
}

void CropSpaceRepr::set_selected(bool selected)
{
    this->selected = selected;
    if (selected) {
        setZValue(1);
    } else {
        setZValue(0);
    }
    update();
}

void CropSpaceRepr::setRect(const QRectF &rect)
{
    prepareGeometryChange();
    this->rect = rect;
    bounding_rect = QRectF();
    update();
}

QRectF CropSpaceRepr::boundingRect() const
{
    if (bounding_rect.isNull()) {
        //qreal halfpw = pen().style() == Qt::NoPen ? qreal(0) : pen().widthF() / 2;
        bounding_rect = rect;
        //if (halfpw > 0.0)
        //    d->boundingRect.adjust(-halfpw, -halfpw, halfpw, halfpw);
    }
    return bounding_rect;
}

void CropSpaceRepr::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *option,
                          QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);
    QPen pen;
    pen.setWidth(0);
    if (selected) {
        QPen white_pen(QColor("white"));
        white_pen.setWidth(0);
        painter->setPen(white_pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect);
        pen.setStyle(Qt::DashLine);
    }
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(rect);
}



SpaceScene::SpaceScene(DatasetModel& dataset_model,
                       CropSelectionController& selection_controller,
                       DateController& date_controller) :
    dataset_model(dataset_model),
    _date_controller(date_controller),
    selection_controller(selection_controller)
{
    QObject::connect(&selection_controller, SIGNAL(selection_changed(Crop*)), this, SLOT(selectCrop(Crop*)));

    QObject::connect(&date_controller, SIGNAL(date_changed(bool)), this, SLOT(update_draw()));

    QObject::connect(&dataset_model, SIGNAL(updated()), this, SLOT(update_draw()));

    selected_subd_repr = 0;
    this->draw_scene();
}

void SpaceScene::update_draw()
{
    this->clear_scene();
    this->draw_scene();
}

void SpaceScene::draw_scene()
{
    for(Plot& plot: this->dataset_model.get_dataset().get_plots())
    {
        if (plot.get_shape())
        {
            PlotRepresentation *plot_repr = new PlotRepresentation(dataset_model.get_dataset().get_crops(),
                                                                   plot, _date_controller.get_date());
            this->addItem(plot_repr);
            plot_reprs.push_back(plot_repr);
        }
    }
    selectCrop(selection_controller.get_selected());
}

void SpaceScene::clear_scene()
{
    selected_subd_repr = 0;
    plot_reprs.clear();
    clear();
}

Crop* SpaceScene::getCropAtPos(QPointF scene_pos)
{
    Crop* p_current_crop = 0;
    for (PlotRepresentation* plot_repr: plot_reprs)
    {
        for (CropSpaceRepr* subd_repr: plot_repr->crop_reprs)
        {
            QRectF subd_rect = subd_repr->sceneTransform().mapRect(subd_repr->boundingRect());
            if (subd_rect.contains(scene_pos))
            {
                if (subd_repr->get_pcrop())
                {
                    p_current_crop = subd_repr->get_pcrop();
                }
                return p_current_crop;
            }
        }
    }
    return p_current_crop;
}

void SpaceScene::removeCropSelection()
{
    if (selected_subd_repr)
    {
        selected_subd_repr->set_selected(false);
    }
    selected_subd_repr = nullptr;
}

void SpaceScene::selectCrop(Crop* p_crop)
{
    removeCropSelection();
    if (p_crop == nullptr) {
        return;
    }

    for (PlotRepresentation* plot_repr: plot_reprs)
    {
        for (CropSpaceRepr* subd_repr: plot_repr->crop_reprs)
        {
            if (subd_repr->get_pcrop() == p_crop)
            {
                selected_subd_repr = subd_repr;
                break;
            }
        }
    }
    if (selected_subd_repr == nullptr) {
        return;
    }

    selected_subd_repr->set_selected(true);
}

void SpaceScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF clic_point = event->scenePos();
    Crop* selected_crop = getCropAtPos(clic_point);
    selection_controller.select_crop(selected_crop);
}

