
#include "EditCropWidget.h"

#include "gui_utils.h"
#include "PlantsModel.h"
#include "PlotsModel.h"

#include "dataset.h"
#include "plot.h"

EditCropWidget::EditCropWidget(Dataset& dataset, PlantsModel* plants_model,
                               PlotsModel* plots_model, QWidget* parent) :
    QWidget(parent), dataset(dataset), p_crop(0), plants_model(plants_model),
    plots_model(plots_model), ui(new Ui::EditCropWidget)
{
    ui->setupUi(this);

    ui->plantInput->setModel(plants_model);
    ui->varInput->setModel(plants_model);
    ui->varInput->setVarRootModelIndex(0);
    QObject::connect(ui->plantInput, SIGNAL(currentIndexChanged(int)),
                     ui->varInput, SLOT(setVarRootModelIndex(int)));

    ui->plotInput->setModel(plots_model);

    QObject::connect(ui->AddButton, SIGNAL(clicked()), this, SLOT(edit_crop()));

    ui->enddateInput->setEnabled(false);
    ui->plannedenddateInput->setEnabled(false);
}


EditCropWidget::~EditCropWidget()
{
    delete ui;
}

void EditCropWidget::set_crop_values(Crop* p_crop)
{
    this->p_crop = p_crop;
    if (p_crop)
    {
        ui->startdateInput->setSelectedDate(toQDate(p_crop->get_date("start")));
        ui->enddateInput->setSelectedDate(toQDate(p_crop->get_date("end")));
        ui->plannedstartdateInput->setSelectedDate(toQDate(p_crop->get_date("planned_start")));
        ui->plannedenddateInput->setSelectedDate(toQDate(p_crop->get_date("planned_end")));
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
    } else {
        //set_default_values();
        //ui->AddButton->show();
        ui->AddButton->setText(tr("Add this crop"));
    }
}


void EditCropWidget::edit_crop()
{
    QString plot_key = ui->plotInput->currentElem();
    Plot* p_plot = dataset.get_pplot(fromQString(plot_key));
    Rectangle rect = ui->shapeInput->get_rect();

    QString plant_key = ui->plantInput->currentElem();
    Plant* p_plant = dataset.get_pplant(fromQString(plant_key));
    QString var_key = ui->varInput->currentElem();
    
    QDate start_date = ui->startdateInput->selectedDate();
    QDate end_date = ui->enddateInput->selectedDate();
    QDate planned_start_date = ui->plannedstartdateInput->selectedDate();
    QDate planned_end_date = ui->plannedenddateInput->selectedDate();

    QString note = ui->noteInput->text();
    if (!p_crop)
    {
        Crop crop = dataset.add_crop(Crop(fromQDate(start_date), fromQDate(end_date),
                                          fromQDate(planned_start_date), fromQDate(planned_end_date),
                                          p_plant, fromQString(var_key), p_plot, fromQString(note)));
    }
    else
    {
        p_crop->set_date("start", fromQDate(start_date));
        p_crop->set_date("end", fromQDate(end_date));
        p_crop->set_date("planned_start", fromQDate(planned_start_date));
        p_crop->set_date("planned_end", fromQDate(planned_end_date));
        p_crop->set_plot(*p_plot);
        p_crop->set_shape(new Rectangle(rect));
        p_crop->set_plant(*p_plant);
        p_crop->set_varkey(fromQString(var_key));
        p_crop->set_note(fromQString(note));
    }
    emit dataset_changed();
}
