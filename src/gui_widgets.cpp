
#include "gui_widgets.h"

#include <QVBoxLayout>
#include <QLabel>

ComboBox::ComboBox(vector<QString> elem_list, QWidget* parent)
{
    this->update(elem_list);
}

void ComboBox::update(vector<QString> elem_list)
{
    this->clear();
    if (elem_list.size())
    {
        this->setEnabled(true);
        for(QString elem: elem_list)
        {
            this->addItem(elem);
        }
    }
    else
    {
        this->setEnabled(false);
    }
}

/*
void ComboBox::setCurrentElem(elem)
{
    if len(self.elem_list) > 0
    {
        self.setCurrentIndex(self.elem_list.index(elem))
    }
}

ComboBox::currentElem(self)
{
    if len(self.elem_list) > 0
    {
        return self.elem_list[self.currentIndex()]
    }
    else
    {
        return None
    }
}
*/


OptionalCalendar::OptionalCalendar(QString title, QWidget* parent):
    QWidget(parent)
{
    this->setLayout(new QVBoxLayout);
    QLabel* label = new QLabel(title);
    checkbox.setText("Activer");
    checkbox.setChecked(true);
    //QObject::connect(&checkbox, SIGNAL(stateChanged(int)), &calendar, SLOT(setEnabled(int)));
    this->layout()->addWidget(label);
    this->layout()->addWidget(&calendar);
    this->layout()->addWidget(&checkbox);
}

void OptionalCalendar::setSelectedDate(QDate date)
{
    if (date.isValid())
    {
        calendar.setSelectedDate(date);
        checkbox.setChecked(true);
    }
    else
    {
        checkbox.setChecked(false);
    }
}

QDate OptionalCalendar::selectedDate()
{
    return calendar.selectedDate();
}

#include <QDialogButtonBox>
EditCropDialog::EditCropDialog(Dataset& dataset, QWidget* parent) :
    QDialog(parent), dataset(dataset), crop(&NullCrop)
{
    setModal(true);

    startdateInput = new OptionalCalendar("Start");
    enddateInput = new OptionalCalendar("End");
    plannedstartdateInput = new OptionalCalendar("Planned start");
    plannedenddateInput = new OptionalCalendar("Planned end");
    plantInput = new ComboBox;
    varInput = new ComboBox;
    plotInput = new ComboBox;
    noteInput = new QLineEdit;
    
    //QObject::connect(plantInput, SIGNAL(currentIndexChanged()), this, SLOT(fill_var()));

    OKbutton = new QPushButton("Modifier");
    QPushButton* Canbutton = new QPushButton("Annuler");
    
    QDialogButtonBox* buttonBox = new QDialogButtonBox();
    buttonBox->addButton(OKbutton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(Canbutton, QDialogButtonBox::RejectRole);
    
    //QObject::connect(Canbutton, SIGNAL(clicked()), this, SLOT(hide()));
    //QObject::connect(OKbutton, SIGNAL(clicked()), this, SLOT(edit_plot()));

    QGridLayout* layout = new QGridLayout;
    this->setLayout(layout);
    layout->addWidget(startdateInput);
    layout->addWidget(enddateInput, 0, 1);
    layout->addWidget(plannedstartdateInput);
    layout->addWidget(plannedenddateInput, 1, 1);
    layout->addWidget(plantInput);
    layout->addWidget(varInput);
    layout->addWidget(plotInput);
    layout->addWidget(noteInput);
    layout->addWidget(buttonBox);
}

void EditCropDialog::set_crop_values(Crop* crop)
{
    this->crop = crop;
    startdateInput->setSelectedDate(toQDate(crop->get_date("start")));
    enddateInput->setSelectedDate(toQDate(crop->get_date("end")));
    plannedstartdateInput->setSelectedDate(toQDate(crop->get_date("planned_start")));
    plannedenddateInput->setSelectedDate(toQDate(crop->get_date("planned_end")));
    //plantInput->setCurrentElem(crop->get_plant());
    //if (crop->get_var_key())
    //{
    //    varInput->setCurrentElem(crop->get_plant().get_varlist()[crop->get_var_key()]);
    //}
    //plotInput->setCurrentElem(crop->get_plot());
    noteInput->setText(toQString(crop->get_note()));
}

void EditCropDialog::set_default_values()
{
    crop = &NullCrop;
}

void EditCropDialog::fill_var()
{
    int plant_index = plantInput->currentIndex();
    if (plant_index >= 0)
    {
        //Plant& plant = dataset.get_plants()[plantInput->currentIndex()];
        //varInput->update(plant.get_vars());
    }
}


void EditCropDialog::edit_plot()
{
  /*
    Plot& plot = plotInput->currentIndex();
    Plant& plant = plantInput->currentIndex();
    Var& var = varInput->currentIndex();
    string var_key;
    if (var)
    {
        var_key = var.key;
    }
    else
    {
        var_key = "";
    }
        
    if (!crop)
    {
        crop = dataset.crops.add(plant, var_key, plot=plot);
    }
    else
    {
        crop->set_plot(plot);
        crop->set_plant(plant);
        crop->get_plant.set_var(var_key);
    }
    crop->start_date = startdateInput->selectedDate();
    crop->end_date = enddateInput->selectedDate();
    crop->planned_start_date = plannedstartdateInput->selectedDate();
    crop->planned_end_date = plannedenddateInput->selectedDate();
    crop->note = noteInput->text();
    emit dataset_changed();
  */
    hide();
}

void EditCropDialog::show()
{
    if (crop)
    {
        //set_crop_values(crop_repr.crop);
        OKbutton->setText("Modifier");
    }
    else
    {
        set_default_values();
        OKbutton->setText("Ajouter");
    }
    show();
}
