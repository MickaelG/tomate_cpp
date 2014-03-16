
#ifndef GUI_EDITCROPDIALOG_H
#define GUI_EDITCROPDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

#include "ComboBoxKey.h"
#include "OptionalCalendar.h"
#include "dataset.h"

class EditCropDialog: public QDialog
{
    Q_OBJECT

public:
    EditCropDialog(Dataset& dataset, QWidget* parent=NULL);
    void set_crop_values(Crop* crop);
    void set_default_values();

private slots:
    void edit_plot();
    void initSubplotInput();
    void initVarInput();

signals:
    void dataset_changed();
    void update_plant_plot();
    
private:
    Dataset& dataset;
    Crop* p_crop;
    OptionalCalendar* startdateInput;
    OptionalCalendar* enddateInput;
    OptionalCalendar* plannedstartdateInput;
    OptionalCalendar* plannedenddateInput;
    ComboBoxKey* plantInput;
    ComboBoxKey* varInput;
    ComboBoxKey* plotInput;
    ComboBoxKey* subplotInput;
    QLineEdit* noteInput;
    QPushButton* OKbutton;
};

#endif //GUI_EDITCROPDIALOG_H
