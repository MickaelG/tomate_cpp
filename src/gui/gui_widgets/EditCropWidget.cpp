
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
    hide();
    ui->setupUi(this);

    ui->plantInput->setModel(plants_model);
    ui->varInput->setModel(plants_model);
    set_var_box(0);
    QObject::connect(ui->plantInput, SIGNAL(currentIndexChanged(int)),
                     this, SLOT(set_var_box(int)));

    ui->plotInput->setModel(plots_model);

    QObject::connect(ui->AddButton, SIGNAL(clicked()), this, SLOT(edit_crop()));
    QObject::connect(ui->DelButton, SIGNAL(clicked()), this, SLOT(delete_crop()));

    ui->enddateInput->setEnabled(false);
    ui->plannedenddateInput->setEnabled(false);

    QObject::connect(&selection_controller, SIGNAL(selection_changed(Crop*)),
                     this, SLOT(set_crop_values(Crop*)));
}

void EditCropWidget::set_var_box(int selected_plant)
{
  ui->varInput->setRootModelIndex(plants_model->index(selected_plant, 0));
  ui->varInput->setCurrentIndex(-1);
}


EditCropWidget::~EditCropWidget()
{
    delete ui;
}

void EditCropWidget::set_crop_values(Crop* crop)
{
    if (crop == nullptr) {
        hide();

        ui->startdateInput->setSelectedDate(QDate::currentDate());
        ui->enddateInput->setSelectedDate(QDate::currentDate());
        ui->enddateInput->setEnabled(false);
        ui->plannedstartdateInput->setSelectedDate(QDate::currentDate());
        ui->plannedstartdateInput->setEnabled(false);
        ui->plannedenddateInput->setSelectedDate(QDate::currentDate());
        ui->plannedenddateInput->setEnabled(false);
        ui->noteInput->clear();
        ui->AddButton->setText(tr("Add crop"));
        ui->DelButton->setText(tr("Cancel"));

        return;
    }
    ui->startdateInput->setSelectedDate(toQDate(crop->get_date(Crop::DateSel::START)));
    ui->enddateInput->setSelectedDate(toQDate(crop->get_date(Crop::DateSel::END)));
    ui->plannedstartdateInput->setSelectedDate(toQDate(crop->get_date(Crop::DateSel::PLANNED_START)));
    ui->plannedenddateInput->setSelectedDate(toQDate(crop->get_date(Crop::DateSel::PLANNED_END)));
    ui->plantInput->setCurrentIndex(plants_model->GetSpeciesRowIndex(crop->get_plant()));
    ui->varInput->setCurrentIndex(plants_model->GetVarietyRowIndex(crop->get_plant()));

    Plot* plot_p = plots_model->get_plots().get_for_pos(*crop->get_shape());
    ui->plotInput->setCurrentIndex(plots_model->GetRowIndex(*plot_p));
    Rectangle crop_shape(*crop->get_shape());
    crop_shape.translate(-plot_p->get_shape()->get_min_x(), -plot_p->get_shape()->get_min_y());
    ui->shapeInput->set_shape(&crop_shape);
    ui->noteInput->setText(toQString(crop->get_note()));

    ui->AddButton->setText(tr("Apply changes"));
    ui->DelButton->setText(tr("Delete crop"));
    show();
}

void EditCropWidget::show_add()
{
    selection_controller.select_crop(nullptr);
    show();
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
    Plot* p_plot = plots_model->GetPlot(ui->plotInput->currentIndex());
    if (p_plot == nullptr) {
       QMessageBox::critical(NULL, QObject::tr("Error"),
                              QObject::tr("Please select a plot."));
       return nullptr;
    }
    Rectangle rect = ui->shapeInput->get_rect();

    Plant* p_plant = plants_model->GetPlant(ui->plantInput->currentIndex(), ui->varInput->currentIndex());
    if (p_plant == nullptr) {
       QMessageBox::critical(NULL, QObject::tr("Error"),
                              QObject::tr("Please select a plant."));
       return nullptr;
    }

    QDate start_date = ui->startdateInput->selectedDate();
    QDate end_date = ui->enddateInput->selectedDate();
    QDate planned_start_date = ui->plannedstartdateInput->selectedDate();
    QDate planned_end_date = ui->plannedenddateInput->selectedDate();

    QString note = ui->noteInput->text();

    rect.fit_in_plot(p_plot->get_shape());
    rect.translate(p_plot->get_shape()->get_min_x(), p_plot->get_shape()->get_min_y());
    CropLocation location(rect);

    unique_ptr<Crop> crop(new Crop(fromQDate(start_date), fromQDate(end_date),
                                   fromQDate(planned_start_date), fromQDate(planned_end_date),
                                   p_plant, location, fromQString(note)));
    return crop;
}

void EditCropWidget::delete_crop()
{
    Crop* selected_crop = selection_controller.get_selected();
    if (selected_crop == nullptr) {
        hide();
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
