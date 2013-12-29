
#include "gui_spaceview.h"

#include "gui_utils.h"

#include <QLayout>
#include <QFont>
#include <QColor>
#include <QBrush>

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
    this->update(date);
}

void PlotRepresentation::update(QDate date)
{
    Rectangle rect = plot.get_rect();
    this->setRect(rect.get_x(), - rect.get_y() - rect.get_height(),
                  rect.get_width(), rect.get_height());
    for (Plot subd: plot.get_subplots())
    {
        Crop& crop = crops.find_crop(subd, fromQDate(date));
        SubdRepresentation *subd_repr = new SubdRepresentation(subd.get_rect(), crop, date);
        subd_repr->setPos(rect.get_x(), -rect.get_y());
        subd_repr->setParentItem(this);
    }
}
//TODO: delete subd_repr
            
SubdRepresentation::SubdRepresentation(Rectangle rect, Crop& crop, QDate date) :
    rect(rect), crop(crop)
{
    this->setRect(get_rect().get_x(), - get_rect().get_y() - get_rect().get_height(),
                  get_rect().get_width(), get_rect().get_height());
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

WholeScene::WholeScene(Dataset& dataset) : dataset(dataset)
{
    date = QDate::currentDate();
    this->draw_scene();
}

void WholeScene::set_date(QDate date)
{
    this->date = date;
    this->update();
}

void WholeScene::update_draw()
{
    this->clear();
    this->draw_scene();
}

void WholeScene::draw_scene()
{
    for(Plot plot: this->dataset.get_plots())
    {
        if (plot.get_rect())
        {
            PlotRepresentation *plot_repr = new PlotRepresentation(dataset.get_crops(),
                                                                   plot, date);
            this->addItem(plot_repr);
            //TODO: delete plot_repr
        }
    }
}
