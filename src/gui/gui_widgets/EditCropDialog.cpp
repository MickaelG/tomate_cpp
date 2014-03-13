
#include "EditCropDialog.h"
#include <QDialogButtonBox>
#include <QGridLayout>

#include "gui_utils.h"
#include "PlantsModel.h"

EditCropDialog::EditCropDialog(Dataset& dataset, QWidget* parent) :
    QDialog(parent), dataset(dataset), p_crop(0)
{
    setModal(true);

    startdateInput = new OptionalCalendar(tr("Start:"));
    enddateInput = new OptionalCalendar(tr("End:"));
    plannedstartdateInput = new OptionalCalendar(tr("Planned start:"));
    plannedenddateInput = new OptionalCalendar(tr("Planned end:"));
    plantInput = new ComboBoxKey(new PlantsModel(dataset.get_plants()));
    plantInput->setToolTip(tr("Plant"));
    varInput = new ComboBoxKey(new VarsModel(dataset.get_plants(), plantInput));
    varInput->setToolTip(tr("Variety"));
    plotInput = new ComboBoxKey(new PlotsModel(dataset.get_plots()));
    plotInput->setToolTip(tr("Plot"));
    subplotInput = new ComboBoxKey(new SubPlotsModel(dataset.get_plots(), plotInput));
    subplotInput->setToolTip(tr("Subplot"));
    noteInput = new QLineEdit;
    noteInput->setPlaceholderText(tr("Note"));
    noteInput->setToolTip(tr("Note"));

    QObject::connect(plantInput, SIGNAL(currentIndexChanged(int)), this->varInput->model(), SIGNAL(layoutChanged()));
    QObject::connect(plotInput, SIGNAL(currentIndexChanged(int)), this->subplotInput->model(), SIGNAL(layoutChanged()));
    QObject::connect(plantInput, SIGNAL(currentIndexChanged(int)), this, SLOT(initVarInput()));
    QObject::connect(plotInput, SIGNAL(currentIndexChanged(int)), this, SLOT(initSubplotInput()));

    OKbutton = new QPushButton(tr("Edit"));
    QPushButton* Canbutton = new QPushButton(tr("Cancel"));
    
    QDialogButtonBox* buttonBox = new QDialogButtonBox();
    buttonBox->addButton(OKbutton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(Canbutton, QDialogButtonBox::RejectRole);
    
    QObject::connect(Canbutton, SIGNAL(clicked()), this, SLOT(hide()));
    QObject::connect(OKbutton, SIGNAL(clicked()), this, SLOT(edit_plot()));

    QGridLayout* layout = new QGridLayout;
    this->setLayout(layout);
    layout->addWidget(startdateInput);
    layout->addWidget(enddateInput, 0, 1);
    layout->addWidget(plannedstartdateInput);
    layout->addWidget(plannedenddateInput, 1, 1);
    layout->addWidget(plantInput);
    layout->addWidget(varInput);
    layout->addWidget(plotInput);
    layout->addWidget(subplotInput);
    layout->addWidget(noteInput, 4, 0, 1, 2);
    layout->addWidget(buttonBox, 5, 1);
}

void EditCropDialog::initSubplotInput()
{
    subplotInput->setCurrentIndex(-1);
    subplotInput->setCurrentIndex(0);
}
void EditCropDialog::initVarInput()
{
    varInput->setCurrentIndex(-1);
    varInput->setCurrentIndex(0);
}

void EditCropDialog::set_crop_values(Crop* p_crop)
{
    this->p_crop = p_crop;
    if (p_crop)
    {
        startdateInput->setSelectedDate(toQDate(p_crop->get_date("start")));
        enddateInput->setSelectedDate(toQDate(p_crop->get_date("end")));
        plannedstartdateInput->setSelectedDate(toQDate(p_crop->get_date("planned_start")));
        plannedenddateInput->setSelectedDate(toQDate(p_crop->get_date("planned_end")));
        plantInput->setCurrentElem(toQString(p_crop->get_plant().get_key()));
        if (p_crop->get_varkey() != "")
        {
            varInput->setCurrentElem(toQString(p_crop->get_varkey()));
        }
        plotInput->setCurrentElem(toQString(p_crop->get_plot().get_key()).split("-")[0]);
        subplotInput->setCurrentElem(toQString(p_crop->get_plot().get_key()));
        noteInput->setText(toQString(p_crop->get_note()));
        OKbutton->setText(tr("Edit"));
    } else {
        //set_default_values();
        OKbutton->setText(tr("Add"));
    }
}


void EditCropDialog::edit_plot()
{
    QString plot_key = subplotInput->currentElem();
    Plot& plot = dataset.get_plot(fromQString(plot_key));
    QString plant_key = plantInput->currentElem();
    Plant& plant = dataset.get_plant(fromQString(plant_key));
    QString var_key = varInput->currentElem();
    
    QDate start_date = startdateInput->selectedDate();
    QDate end_date = enddateInput->selectedDate();
    QDate planned_start_date = plannedstartdateInput->selectedDate();
    QDate planned_end_date = plannedenddateInput->selectedDate();
    QString note = noteInput->text();
    if (!p_crop)
    {
        Crop crop = dataset.add_crop(Crop(fromQDate(start_date), fromQDate(end_date),
                                          fromQDate(planned_start_date), fromQDate(planned_end_date),
                                          plant, fromQString(var_key), plot, fromQString(note)));
    }
    else
    {
        p_crop->set_date("start", fromQDate(start_date));
        p_crop->set_date("end", fromQDate(end_date));
        p_crop->set_date("planned_start", fromQDate(planned_start_date));
        p_crop->set_date("planned_end", fromQDate(planned_end_date));
        p_crop->set_plot(plot);
        p_crop->set_plant(plant);
        p_crop->set_varkey(fromQString(var_key));
        p_crop->set_note(fromQString(note));
    }
    emit dataset_changed();
    hide();
}

