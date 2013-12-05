
#ifndef GUI_WIDGETS_H
#define GUI_WIDGETS_H

#include <QDialog>
#include <QComboBox>
#include <QCalendarWidget>
#include <QCheckBox>
#include <QListView>
#include <QLineEdit>
#include <QPushButton>

#include "dataset.h"
#include "gui_utils.h"


class ComboBox: public QComboBox
{
    //QtWidget to show a list of element and let the user chose one
    //(based on QComboBox).
    Q_OBJECT
public:
    ComboBox(vector<QString> elem_list = vector<QString>(), QWidget* parent = 0);
    void update(vector<QString> elem_list);
};

class OptionalCalendar: public QWidget
{
    Q_OBJECT

public:
    OptionalCalendar(QString title, QWidget* parent=NULL);
    void setSelectedDate(QDate date=QDate());
    QDate selectedDate();
private:
    QCalendarWidget calendar;
    QCheckBox checkbox;
};

class EditCropDialog: public QDialog
{
    Q_OBJECT

public:
    EditCropDialog(Dataset& dataset, QWidget* parent=NULL);
    void set_crop_values(Crop* crop);
    void set_default_values();
    void fill_var();
    void edit_plot();
    void show();
    
signals:
    void dataset_changed();
    
private:
    Dataset& dataset;
    Crop* crop;
    OptionalCalendar* startdateInput;
    OptionalCalendar* enddateInput;
    OptionalCalendar* plannedstartdateInput;
    OptionalCalendar* plannedenddateInput;
    ComboBox* plantInput;
    ComboBox* varInput;
    ComboBox* plotInput;
    QLineEdit* noteInput;
    QPushButton* OKbutton;
};


class AddCropDialog: public QDialog
{
    Q_OBJECT

public:
    AddCropDialog(Dataset& dataset, QWidget* parent=NULL);
};

class ListModel: public QAbstractListModel
{
    Q_OBJECT

public:
    ListModel(vector<QString> elem_list, QWidget* parent = 0);

private:
    vector<QString> elem_list;
};

class ListWidget: public QListView
{
    Q_OBJECT

public:
    ListWidget(vector<QString> elem_list, QWidget* parent=NULL);
    void index();
};

#endif //GUI_WIDGETS_H
