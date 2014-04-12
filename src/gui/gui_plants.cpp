
#include "gui_plants.h"
#include "gui_utils.h"

#include "plants.h"
#include "gui_widgets/PlantsModel.h"

#include <QColorDialog>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>



AddDialog::AddDialog(PlantsModel* plants_model, QWidget* parent) :
  plants_model(plants_model), QDialog(parent)
{
    setModal(true);
    QFormLayout* formLayout = new QFormLayout(this);
    nameInput = new QLineEdit();
    formLayout->addRow(tr("Name :"), nameInput);
    QPushButton* OKbutton = new QPushButton(tr("Add"));
    QPushButton* Canbutton = new QPushButton(tr("Cancel"));
    QDialogButtonBox* buttonBox = new QDialogButtonBox();
    buttonBox->addButton(OKbutton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(Canbutton, QDialogButtonBox::RejectRole);
    formLayout->addRow(buttonBox);
    QObject::connect(Canbutton, SIGNAL(clicked()), this, SLOT(hide()));
    QObject::connect(OKbutton, SIGNAL(clicked()), this, SLOT(add()));
}


void AddDialog::add()
{
    QString name = nameInput->text();
    add_elem(name);
    hide();
}


class AddPlantDialog: public AddDialog
{
public:
    AddPlantDialog(PlantsModel* plants_model, QWidget* parent=NULL);
    void add_elem(QString name);

};

AddPlantDialog::AddPlantDialog(PlantsModel *plants_model, QWidget *parent): AddDialog(plants_model, parent)
{
}

void AddPlantDialog::add_elem(QString name)
{
    plants_model->addPlant(name);
}


class AddVarDialog: public AddDialog
{
public:
    AddVarDialog(PlantsModel* plants_model, QListView* plants_widget, QWidget* parent=NULL);
    void add_elem(QString name);
    
private:
    QListView* plants_widget;
};


AddVarDialog::AddVarDialog(PlantsModel *plants_model, QListView* plants_widget, QWidget* parent) :
  plants_widget(plants_widget), AddDialog(plants_model, parent)
{
}


void AddVarDialog::add_elem(QString name)
{
    const QModelIndex& plant_index = plants_widget->selectionModel()->currentIndex();
    plants_model->addVar(plant_index, name);
}


PlantsWindow::PlantsWindow(PlantsModel* plants_model, QWidget* parent) :
  QDialog(parent), plants_model(plants_model)
{
    setModal(true);

    color_dialog = new QColorDialog;
    QVBoxLayout* plants_layout = new QVBoxLayout();
    plants_widget = new QListView(this);
    plants_widget->setModel(plants_model);
    plants_layout->addWidget(plants_widget);
    QPushButton* add_plant_btn = new QPushButton(tr("Add a plant"));
    del_plant_btn = new QPushButton(tr("Delete plant"));
    del_plant_btn->setEnabled(false);
    AddDialog* add_plant = new AddPlantDialog(plants_model);
    QObject::connect(add_plant_btn, SIGNAL(clicked()), add_plant, SLOT(show()));
    QObject::connect(del_plant_btn, SIGNAL(clicked()), this, SLOT(delete_plant()));
    plants_layout->addWidget(add_plant_btn);
    plants_layout->addWidget(del_plant_btn);

    QPushButton* close_btn = new QPushButton(tr("Close"));
    plants_layout->addWidget(close_btn);
    QObject::connect(close_btn, SIGNAL(clicked()), this, SLOT(accept()));

    QVBoxLayout* var_layout = new QVBoxLayout();
    var_widget = new QListView(this);
    var_widget->setModel(plants_model);
    var_widget->setRootIndex(plants_model->index(0,0));
    var_layout->addWidget(var_widget);
    add_var_btn = new QPushButton(tr("Add a variety"));
    AddVarDialog* add_var = new AddVarDialog(plants_model, plants_widget);
    QObject::connect(add_var_btn, SIGNAL(clicked()), add_var, SLOT(show()));
    var_layout->addWidget(add_var_btn);

    QObject::connect(plants_widget->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
                     var_widget, SLOT(setRootIndex(const QModelIndex&)));
    QObject::connect(plants_widget->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
                     this, SLOT(update_plant_data(const QModelIndex&, const QModelIndex&)));

    QVBoxLayout* notes_layout = new QVBoxLayout();
    notes_widget = new QTextEdit();
    notes_layout->addWidget(notes_widget);
    //update_notes();

    color_btn = new QPushButton(tr("Color"));
    QObject::connect(color_btn, SIGNAL(clicked()), this, SLOT(set_color()));

    QVBoxLayout* details_layout = new QVBoxLayout();
    details_layout->addLayout(var_layout);
    details_layout->addWidget(color_btn);
    details_layout->addLayout(notes_layout);

    QHBoxLayout* main_layout = new QHBoxLayout(this);
    main_layout->addLayout(plants_layout);
    main_layout->addLayout(details_layout);
    
    update_btns_state();
}


Plant& PlantsWindow::selected_plant()
{
    int plant_index = plants_widget->selectionModel()->currentIndex().row();
    return plants_model->get_plants().index(plant_index);
}

void PlantsWindow::set_color()
{
    QString color_str = toQString(selected_plant().get_color_str());
    QColor init_color(color_str);
    QColor color = color_dialog->getColor(init_color, this);
    selected_plant().set_color_str(fromQString(color.name()));
    emit timeline_need_update();
}

void PlantsWindow::delete_plant()
{
    plants_widget->model()->removeRow(plants_widget->selectionModel()->currentIndex().row());
    emit timeline_need_update();
}

void PlantsWindow::update_plant_data(const QModelIndex& current_plant_mi, const QModelIndex& previous_plant_mi)
{
    int previous_plant_index = previous_plant_mi.row();
    int current_plant_index = current_plant_mi.row();
    update_btns_state();
    update_notes(current_plant_index, previous_plant_index);
    update_del_btn(current_plant_index);
    emit timeline_need_update();
}

void PlantsWindow::update_del_btn(int current_plant_index)
{
    if (plants_model->get_plants().is_used(current_plant_index)) {
        del_plant_btn->setEnabled(false);
    } else {
        del_plant_btn->setEnabled(true);
    }
}

void PlantsWindow::update_btns_state()
{
    int plant_index = plants_widget->selectionModel()->currentIndex().row();
    if (plant_index < 0)
    {
        add_var_btn->setDisabled(true);
        color_btn->setDisabled(true);
        notes_widget->setDisabled(true);
    }
    else
    {
        add_var_btn->setDisabled(false);
        color_btn->setDisabled(false);
        notes_widget->setDisabled(false);
    }
}

void PlantsWindow::update_notes(int current_plant_index, int previous_plant_index)
{
    //We save the note content of the previous plant
    if (previous_plant_index >= 0)
    {
        QString text = notes_widget->toPlainText();
        plants_model->get_plants().index(previous_plant_index).set_note(fromQString(text));
    }

    //and get the notes for the new plant
    if (current_plant_index >= 0)
    {
        QString text = toQString(plants_model->get_plants().index(current_plant_index).get_note());
        notes_widget->setPlainText(text);
    }
}
