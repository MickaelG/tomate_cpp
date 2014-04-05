
#include "gui_spaceview.h"

#include "gui_utils.h"

#include "dataset.h"
#include "plot.h"

#include <QLayout>
#include <QFont>
#include <QColor>
#include <QBrush>
#include <QGraphicsSceneMouseEvent>

SpaceViewWindow::SpaceViewWindow(Dataset& dataset) : view(dataset, this)
{
    this->setLayout(new QGridLayout);
    this->layout()->addWidget(&view);
}

void SpaceViewWindow::update_draw()
{
    view.update_draw();
}
                
WholeSceneView::WholeSceneView(Dataset& dataset, QWidget* parent):
  my_scene(dataset)
{
    setScene(&my_scene);
    zoom_fit();
}

void WholeSceneView::update_draw()
{
    my_scene.update_draw();
    zoom_fit();
}

void WholeSceneView::zoom_fit()
{
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}

void WholeSceneView::resizeEvent(QResizeEvent *event)
{
    zoom_fit();
}

PlotRepresentation::PlotRepresentation(Crops& crops, Plot& plot, QDate date) :
    crops(crops), plot(plot)
{
    this->update_draw(date);
}

void PlotRepresentation::update_draw(QDate date)
{
    Shape* shape = plot.get_shape();
    this->setRect(shape->get_min_x(), - shape->get_min_y() - shape->get_height(),
                  shape->get_width(), shape->get_height());
    for (Crop* p_crop: crops.find_crops(plot, fromQDate(date)))
    {
        Crop& crop = *p_crop;
        CropSpaceRepr *crop_repr = new CropSpaceRepr(crop, date);
        crop_repr->setPos(shape->get_min_x(), -shape->get_min_y());
        crop_repr->setParentItem(this);
        crop_reprs.push_back(crop_repr);
    }
}
//TODO: delete subd_repr
            
CropSpaceRepr::CropSpaceRepr(Crop& crop, QDate date) :
    crop(crop)
{
    Shape* shape = crop.get_shape();
    if (!shape)
    {
        return;
    }

    //TODO draw the real shape instead of a rect
    this->setRect(shape->get_min_x(), - shape->get_min_y() - shape->get_height(),
                  shape->get_width(), shape->get_height());
    if (crop)
    {
        Plant& plant = crop.get_plant();
        QString text = toQString(plant.get_name());
        //text = re.sub("\s+", "\n", text)
        QGraphicsSimpleTextItem *textw =  new QGraphicsSimpleTextItem(text);
        QFont font = textw->font();
        font.setPointSize(6);
        font.setWeight(25);
        textw->setFont(font);
        center_text(textw, this->boundingRect());
        textw->setParentItem(this);
        QString color_str = toQString(plant.get_color_str());
        if (color_str == "") { color_str = "#FF00FF"; }
        QColor color = QColor(color_str);
        if (crop.is_planned_at_date(fromQDate(date)) && !crop.is_active_at_date(fromQDate(date)))
        {
            this->setBrush(QBrush(color, Qt::BDiagPattern));
        }
        else
        {
            this->setBrush(QBrush(color));
        }
    }
}

Crop* CropSpaceRepr::get_pcrop()
{
    return &crop;
}

WholeScene::WholeScene(Dataset& dataset) : dataset(dataset)
{
    date = QDate::currentDate();
    selected_subd_repr = 0;
    this->draw_scene();
}

void WholeScene::set_date(QDate date)
{
    this->date = date;
    this->update_draw();
}

void WholeScene::update_draw()
{
    this->clear_scene();
    this->draw_scene();
}

void WholeScene::draw_scene()
{
    for(Plot plot: this->dataset.get_plots())
    {
        if (plot.get_shape())
        {
            PlotRepresentation *plot_repr = new PlotRepresentation(dataset.get_crops(),
                                                                   plot, date);
            this->addItem(plot_repr);
            plot_reprs.push_back(plot_repr);
            //TODO: delete plot_repr
        }
    }
}

void WholeScene::clear_scene()
{
    selected_subd_repr = 0;
    plot_reprs.clear();
    clear();
}

Crop* WholeScene::getCropAtPos(QPointF scene_pos)
{
    Crop* p_current_crop = 0;
    for (PlotRepresentation* plot_repr: plot_reprs)
    {
        for (CropSpaceRepr* subd_repr: plot_repr->crop_reprs)
        {
            QRectF subd_rect = subd_repr->sceneTransform().mapRect(subd_repr->boundingRect());
            if (subd_rect.contains(scene_pos))
            {
                if (subd_repr->get_pcrop() != &NullCrop)
                {
                    p_current_crop = subd_repr->get_pcrop();
                }
                return p_current_crop;
            }
        }
    }
    return p_current_crop;
}

void WholeScene::drawCropSelection()
{
    if (selected_crop && !selected_subd_repr)
    {
        for (PlotRepresentation* plot_repr: plot_reprs)
        {
            for (CropSpaceRepr* subd_repr: plot_repr->crop_reprs)
            {
                if (subd_repr->get_pcrop() == selected_crop)
                {
                    selected_subd_repr = subd_repr;
                    break;
                }
            }
        }
    }
    if (selected_subd_repr)
    {
        QPen selected_pen;
        float scene_scale = views()[0]->transform().m11();
        float pen_width = 4 / scene_scale;
        selected_pen.setWidthF(pen_width);
        //selected_pen.setColor(QColor("#444444"));
        selected_subd_repr->setZValue(1);
        selected_subd_repr->setPen(selected_pen);
    }
}

void WholeScene::removeCropSelection()
{
    if (selected_subd_repr)
    {
        selected_subd_repr->setZValue(0);
        selected_subd_repr->setPen(QPen());
    }
}

void WholeScene::selectCrop(Crop* p_crop)
{
    removeCropSelection();
    selected_subd_repr = 0;
    selected_crop = p_crop;
    drawCropSelection();
}

void WholeScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF clic_point = event->scenePos();
    Crop* p_current_crop = getCropAtPos(clic_point);
    selectCrop(p_current_crop);
    emit crop_selected(selected_crop);
}
