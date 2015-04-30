
#ifndef GUI_EDITCROPWIDGET_H
#define GUI_EDITCROPWIDGET_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

#include "ui_EditCropWidget.h"

class Dataset;
class Crop;
class PlantsModel;
class PlotsModel;

class EditCropWidget: public QWidget
{
    Q_OBJECT

public:
    EditCropWidget(Dataset& dataset, PlantsModel *plants_model,
                   PlotsModel *plots_model, QWidget* parent=NULL);
    ~EditCropWidget();
    void set_default_values();
    Ui::EditCropWidget *ui;

private slots:
    void set_crop_values(Crop *p_crop);
    void edit_crop();
    void delete_crop();

signals:
    void dataset_changed();
    void select_crop(Crop* p_crop);

private:
    PlantsModel* plants_model;
    PlotsModel* plots_model;
    Dataset& dataset;
    Crop* p_crop;
};

#endif //GUI_EDITCROPWIDGET_H
