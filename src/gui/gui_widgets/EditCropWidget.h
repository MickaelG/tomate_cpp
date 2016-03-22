
#ifndef GUI_EDITCROPWIDGET_H
#define GUI_EDITCROPWIDGET_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

#include <memory>

#include "ui_EditCropWidget.h"

class DatasetModel;
class DatasetController;
class CropSelectionController;
class Crop;
class PlantsModel;
class PlotsModel;

class EditCropWidget: public QWidget
{
    Q_OBJECT

public:
    EditCropWidget(DatasetModel& dataset_model,
                   DatasetController& dataset_controller,
                   CropSelectionController& selection_controller,
                   PlantsModel *plants_model,
                   PlotsModel *plots_model,
                   QWidget* parent=NULL);
    ~EditCropWidget();
    void set_default_values();
    Ui::EditCropWidget *ui;

private slots:
    void set_crop_values(Crop *crop);
    void edit_crop();
    void delete_crop();
    void show_add();

private:
    std::unique_ptr<Crop> get_described_crop();

    PlantsModel* plants_model;
    PlotsModel* plots_model;
    DatasetModel& dataset_model;
    DatasetController& dataset_controller;
    CropSelectionController& selection_controller;
};

#endif //GUI_EDITCROPWIDGET_H
