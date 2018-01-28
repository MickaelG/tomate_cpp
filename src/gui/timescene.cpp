
#include "log.h"
#include "timescene.h"
#include "gui_utils.h"
#include "gui_controller.h"
#include "DateRuler.h"

#include "dataset.h"
#include "plot.h"

#include "partitionset.h"

#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsProxyWidget>
#include <QGridLayout>
#include <QColor>
#include <QPushButton>
#include <QSignalMapper>


TimeScene::TimeScene(DatasetModel& dataset_model,
                     CropSelectionController& selection_controller,
                     DateController& date_controller,
                     QWidget* parent) :
    QGraphicsScene(parent),
    dataset_model(dataset_model),
    selected_crop_repr(nullptr),
    _selection_controller(selection_controller),
    _date_controller(date_controller),
    _date_ruler(nullptr)
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
    redraw();
    /*
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
    */
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
    CropTimeRepresentation* p_current_crop_repr = getCropReprAtPos(clic_point);
    if (p_current_crop_repr != nullptr) {
        emit crop_selected(p_current_crop_repr->get_pcrop());
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

void TimeScene::date_changed(bool year_changed) {
    if (year_changed) {
        redraw();
    } else {
        _date_ruler->update_date();
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

pair<float, float> compute_crop_ypos(const Crop& crop, const Plot& plot)
{
    float pos = 0;
    float height = crop.get_shape().get_area() / plot.get_shape().get_area();

    return make_pair(pos, height);
}


void TimeScene::draw_scene()
{
    add_crops();

    _date_ruler = new DateRuler(_date_controller, 24);
    addItem(_date_ruler);

    update();
    emit size_changed();
}


void TimeScene::add_crops()
{
    Crop* selected_crop = _selection_controller.get_selected();

    if (selected_crop == nullptr) {
        return;
    }

    const float full_height = 24;

    QDate date0 = QDate(_date_controller.get_date().year(), 1, 1);
    Crops& crops = dataset_model.get_dataset().get_crops();

    CropTimeRepresentation* crop_repr = new CropTimeRepresentation(*selected_crop, 0, full_height, date0);
    crop_repr->set_selected(true);
    crop_reprs.push_back(crop_repr);
    addItem(crop_repr);
    const float selected_area = selected_crop->get_shape().get_area();

    //Select crops in current year and current plot
    for (Crop& crop: crops)
    {
        if (&crop == selected_crop) {
            continue;
        }
        if (!crop.is_in_year_started_by(fromQDate(date0))) {
            continue;
        }
        if (!crop.get_shape().overlaps(selected_crop->get_shape())) {
            continue;
        }
        unique_ptr<Shape> overlapping_shape(crop.get_shape().clone());
        overlapping_shape->fit_in_other(selected_crop->get_shape());
        const float overlapping_area = overlapping_shape->get_area();
        const float height = full_height * overlapping_area / selected_area;
        CropTimeRepresentation* crop_repr = new CropTimeRepresentation(crop, 0, height, date0);
        crop_reprs.push_back(crop_repr);
        addItem(crop_repr);
    }
}

