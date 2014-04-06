
#include "EditCropWidget.h"

#include "gui_utils.h"
#include "PlantsModel.h"

#include "dataset.h"
#include "plot.h"

EditCropWidget::EditCropWidget(Dataset& dataset, QWidget* parent) :
    QWidget(parent), dataset(dataset), p_crop(0),
    ui(new Ui::EditCropWidget)
{
    ui->setupUi(this);

    QObject::connect(ui->plantInput, SIGNAL(currentIndexChanged(int)), ui->varInput->model(), SIGNAL(layoutChanged()));
    QObject::connect(ui->plantInput, SIGNAL(currentIndexChanged(int)), this, SLOT(initVarInput()));
    QObject::connect(this, SIGNAL(update_plant_plot()), ui->plantInput->model(), SIGNAL(layoutChanged()));
    QObject::connect(this, SIGNAL(update_plant_plot()), ui->plotInput->model(), SIGNAL(layoutChanged()));
    QObject::connect(ui->AddButton, SIGNAL(clicked()), this, SLOT(edit_crop()));

    ui->plantInput->setModel(new PlantsModel(dataset.get_plants()));
    ui->varInput->setModel(new VarsModel(dataset.get_plants(), ui->plantInput));
    ui->plotInput->setModel(new PlotsModel(dataset.get_plots()));
    ui->enddateInput->setEnabled(false);
    ui->plannedenddateInput->setEnabled(false);
}


EditCropWidget::~EditCropWidget()
{
    delete ui;
}


void EditCropWidget::initVarInput()
{
    ui->varInput->setCurrentIndex(-1);
    ui->varInput->setCurrentIndex(0);
}

void EditCropWidget::set_crop_values(Crop* p_crop)
{
    emit update_plant_plot();

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
    Plot& plot = dataset.get_plot(fromQString(plot_key));
    Rectangle rect = ui->shapeInput->get_rect();

    QString plant_key = ui->plantInput->currentElem();
    Plant& plant = dataset.get_plant(fromQString(plant_key));
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
                                          plant, fromQString(var_key), plot, fromQString(note)));
    }
    else
    {
        p_crop->set_date("start", fromQDate(start_date));
        p_crop->set_date("end", fromQDate(end_date));
        p_crop->set_date("planned_start", fromQDate(planned_start_date));
        p_crop->set_date("planned_end", fromQDate(planned_end_date));
        p_crop->set_plot(plot);
        p_crop->set_shape(new Rectangle(rect));
        p_crop->set_plant(plant);
        p_crop->set_varkey(fromQString(var_key));
        p_crop->set_note(fromQString(note));
    }
    emit dataset_changed();
}

void EditCropWidget::on_EditPlantsBtn_clicked()
{

}
