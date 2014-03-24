
#ifndef GUI_EDITCROPWIDGET_H
#define GUI_EDITCROPWIDGET_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

#include "ComboBoxKey.h"
#include "dataset.h"

#include "ui_EditCropWidget.h"

class EditCropWidget: public QWidget
{
    Q_OBJECT

public:
    EditCropWidget(Dataset& dataset, QWidget* parent=NULL);
    ~EditCropWidget();
    void set_default_values();

private slots:
    void set_crop_values(Crop* p_crop);
    void edit_crop();
    void initSubplotInput();
    void initVarInput();

signals:
    void dataset_changed();
    void update_plant_plot();
    
private:
    Ui::EditCropWidget *ui;
    Dataset& dataset;
    Crop* p_crop;
};

#endif //GUI_EDITCROPWIDGET_H
