
#include "gui_controller.h"

#include "crops.h"
#include "dataset.h"

void CropSelectionController::select_crop(Crop *p_crop)
{
    _selected_crop = p_crop;
    emit selection_changed(p_crop);
}

Crop* CropSelectionController::get_selected()
{
    return _selected_crop;
}

void DatasetController::remove_crop(Crop* crop) {
    if (crop == nullptr) {
        return;
    }
    if (selection_controller.get_selected() == crop) {
        selection_controller.select_crop(nullptr);
    }
    model.get_dataset().get_crops().remove(*crop);
    model.emit_updated();
}

void DatasetController::update_current_crop(unique_ptr< Crop > new_crop)
{
    Crop* current_crop = selection_controller.get_selected();
    if (current_crop == nullptr) {
        current_crop = &model.get_dataset().get_crops().add(std::move(new_crop));
        selection_controller.select_crop(current_crop);
    } else {
        *current_crop = std::move(*new_crop);
    }
    model.emit_updated();
}
