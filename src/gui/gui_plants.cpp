
#include "gui_plants.h"
#include "gui_utils.h"

#include <QColorDialog>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>



AddDialog::AddDialog(Plants& plants, QWidget* parent) :
  plants(plants), QDialog(parent)
{
    setModal(true);
    QFormLayout* formLayout = new QFormLayout(this);
    keyInput = new QLineEdit();
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
    emit list_updated();
    hide();
}


class AddPlantDialog: public AddDialog
{
public:
    AddPlantDialog(Plants& plants);
    void add_elem(QString name);

};

AddPlantDialog::AddPlantDialog(Plants& plants): AddDialog(plants)
{
}

void AddPlantDialog::add_elem(QString name)
{
    plants.add_plant("", fromQString(name));
}


class AddVarDialog: public AddDialog
{
public:
    AddVarDialog(Plants& plants, ListWidget* plants_widget, QWidget* parent=NULL);
    void add_elem(QString name);
    
private:
    ListWidget* plants_widget;
};


AddVarDialog::AddVarDialog(Plants& plants, ListWidget* plants_widget, QWidget* parent) :
  plants_widget(plants_widget), AddDialog(plants, parent)
{
}


void AddVarDialog::add_elem(QString name)
{
    int plant_index = plants_widget->index();
    plants.index(plant_index).add_var("", fromQString(name));
}

vector<QString> PlantsWindow::plants_str_list()
{
    vector<QString> result;
    
    for (Plants::iterator it = plants.begin(); it !=plants.end(); ++it)
    {
        result.push_back(toQString(it->get_name()));
    }
    return result;
}

PlantsWindow::PlantsWindow(Plants& plants, QWidget* parent) :
  QWidget(parent), plants(plants)
{
    color_dialog = new QColorDialog;
    QVBoxLayout* plants_layout = new QVBoxLayout();
    vector<QString> plants_str = plants_str_list();
    plants_widget = new ListWidget(new PlantsModel(plants));
    //plants_widget = new ListWidget(new KeyNamesModel(plants));
    plants_layout->addWidget(plants_widget);
    QPushButton* add_plant_btn = new QPushButton(tr("Add a plant"));
    AddDialog* add_plant = new AddPlantDialog(plants);
    QObject::connect(add_plant_btn, SIGNAL(clicked()), add_plant, SLOT(show()));
    plants_layout->addWidget(add_plant_btn);

    QVBoxLayout* var_layout = new QVBoxLayout();
    var_widget = new QListView();
    var_widget->setModel(new VarsModel(plants, plants_widget));
    var_layout->addWidget(var_widget);
    add_var_btn = new QPushButton(tr("Add a variety"));
    AddVarDialog* add_var = new AddVarDialog(plants, plants_widget);
    QObject::connect(add_var_btn, SIGNAL(clicked()), add_var, SLOT(show()));
    var_layout->addWidget(add_var_btn);

    QObject::connect(add_var, SIGNAL(list_updated()), var_widget->model(), SIGNAL(layoutChanged()));
    QObject::connect(add_plant, SIGNAL(list_updated()), plants_widget->model(), SIGNAL(layoutChanged()));
    QObject::connect(plants_widget->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
                     var_widget->model(), SIGNAL(layoutChanged()));
    QObject::connect(plants_widget->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
                     this, SLOT(update_notes(const QModelIndex&, const QModelIndex&)));
    QObject::connect(plants_widget->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
                     this, SLOT(update_btns_state()));

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


void PlantsWindow::update_btns_state()
{
    if (plants_widget->index() < 0)
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

Plant& PlantsWindow::selected_plant()
{
    int plant_index = plants_widget->selectionModel()->currentIndex().row();
    return plants.index(plant_index);
}

void PlantsWindow::set_color()
{
    QString color_str = toQString(selected_plant().get_color_str());
    QColor init_color(color_str);
    QColor color = color_dialog->getColor(init_color, this);
    selected_plant().set_color_str(fromQString(color.name()));
    emit timeline_need_update();
}


void PlantsWindow::update_notes(const QModelIndex& current_plant_mi, const QModelIndex& previous_plant_mi)
{
    int previous_plant_index = previous_plant_mi.row();
    //We save the note content of the previous plant
    if (previous_plant_index >= 0)
    {
        QString text = notes_widget->toPlainText();
        plants.index(previous_plant_index).set_note(fromQString(text));
    }

    //and get the notes for the new plant
    int current_plant_index = current_plant_mi.row();
    if (current_plant_index >= 0)
    {
        QString text = toQString(plants.index(current_plant_index).get_note());
        notes_widget->setPlainText(text);
    }
}
