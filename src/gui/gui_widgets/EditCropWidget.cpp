
#include "EditCropWidget.h"

#include "gui_utils.h"
#include "gui_controller.h"
#include "PlantsModel.h"
#include "PlotsModel.h"

#include "dataset.h"
#include "plot.h"

#include <QMessageBox>
#include <string>

EditCropWidget::EditCropWidget(DatasetModel& dataset_model,
                               DatasetController& dataset_controller,
                               CropSelectionController& selection_controller,
                               PlantsModel* plants_model,
                               PlotsModel* plots_model, QWidget* parent) :
    QWidget(parent),
    dataset_model(dataset_model),
    dataset_controller(dataset_controller),
    selection_controller(selection_controller),
    plants_model(plants_model),
    plots_model(plots_model),
    ui(new Ui::EditCropWidget)
{
    ui->setupUi(this);

    ui->plantInput->setModel(plants_model);
    ui->varInput->setModel(plants_model);
    ui->varInput->setVarRootModelIndex(0);
    QObject::connect(ui->plantInput, SIGNAL(currentIndexChanged(int)),
                     ui->varInput, SLOT(setVarRootModelIndex(int)));

    ui->plotInput->setModel(plots_model);

    QObject::connect(ui->AddButton, SIGNAL(clicked()), this, SLOT(edit_crop()));
    QObject::connect(ui->DelButton, SIGNAL(clicked()), this, SLOT(delete_crop()));

    ui->enddateInput->setEnabled(false);
    ui->plannedenddateInput->setEnabled(false);

    ui->DelButton->setEnabled(false);

    QObject::connect(&selection_controller, SIGNAL(selection_changed(Crop*)),
                     this, SLOT(set_crop_values(Crop*)));
}


EditCropWidget::~EditCropWidget()
{
    delete ui;
}

void EditCropWidget::set_crop_values(Crop* p_crop)
{
    if (p_crop)
    {
        ui->startdateInput->setSelectedDate(toQDate(p_crop->get_date(Crop::DateSel::START)));
        ui->enddateInput->setSelectedDate(toQDate(p_crop->get_date(Crop::DateSel::END)));
        ui->plannedstartdateInput->setSelectedDate(toQDate(p_crop->get_date(Crop::DateSel::PLANNED_START)));
        ui->plannedenddateInput->setSelectedDate(toQDate(p_crop->get_date(Crop::DateSel::PLANNED_END)));
        ui->plantInput->setCurrentElem(toQString(p_crop->get_plant().get_key()));
        if (p_crop->get_varkey() != "")
        {
            ui->varInput->setCurrentElem(toQString(p_crop->get_varkey()));
        }
        ui->plotInput->setCurrentElem(toQString(p_crop->get_plot().get_key()).split("-")[0]);
        ui->shapeInput->set_shape(p_crop->get_shape());
        ui->noteInput->setText(toQString(p_crop->get_note()));
        //ui->AddButton->hide();
        ui->AddButton->setText(tr("Apply changes"));
        ui->DelButton->setEnabled(true);
    } else {
        //set_default_values();
        //ui->AddButton->show();
        ui->AddButton->setText(tr("Add this crop"));
        ui->DelButton->setEnabled(false);
    }
}

void EditCropWidget::edit_crop()
{
    unique_ptr<Crop> crop(get_described_crop());
    if (crop.get() != nullptr) {
        dataset_controller.update_current_crop(*crop);
    }
}

unique_ptr<Crop> EditCropWidget::get_described_crop()
{
    QString plot_key = ui->plotInput->currentElem();
    Plot* p_plot = dataset_model.get_dataset().get_plots().find(fromQString(plot_key));
    if (p_plot == nullptr) {
       QMessageBox::critical(NULL, QObject::tr("Error"),
                              QObject::tr("Please select a plot."));
       return nullptr;
    }
    Rectangle rect = ui->shapeInput->get_rect();

    QString plant_key = ui->plantInput->currentElem();
    Plant* p_plant = dataset_model.get_dataset().get_plants().find(fromQString(plant_key));
    if (p_plant == nullptr) {
       QMessageBox::critical(NULL, QObject::tr("Error"),
                              QObject::tr("Please select a plant."));
       return nullptr;
    }
    QString var_key = ui->varInput->currentElem();

    QDate start_date = ui->startdateInput->selectedDate();
    QDate end_date = ui->enddateInput->selectedDate();
    QDate planned_start_date = ui->plannedstartdateInput->selectedDate();
    QDate planned_end_date = ui->plannedenddateInput->selectedDate();

    QString note = ui->noteInput->text();

    unique_ptr<Crop> crop(new Crop(fromQDate(start_date), fromQDate(end_date),
                                   fromQDate(planned_start_date), fromQDate(planned_end_date),
                                   p_plant, fromQString(var_key), p_plot, fromQString(note),
                                   rect));
    return crop;
}

void EditCropWidget::delete_crop()
{
    Crop* selected_crop = selection_controller.get_selected();
    if (selected_crop == nullptr) {
        return;
    }
    int result = QMessageBox::question(NULL, QObject::tr("Delete crop"),
                                       QObject::tr("Are you sure you want to delete the crop ?"),
                                       QMessageBox::Yes | QMessageBox::No);

    if (result != QMessageBox::Yes) {
        return;
    }

    dataset_controller.remove_crop(selected_crop);
}
