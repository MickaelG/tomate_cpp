
#include "gui_plots.h"
#include "gui_utils.h"

#include "plots.h"
#include "plot.h"
#include "gui_widgets/PhysInput.h"
#include "PlotsModel.h"

#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>


AddPlotDialog::AddPlotDialog(PlotsModel* plots_model, QWidget* parent) :
  plots_model(plots_model), QDialog(parent)
{
    setModal(true);
    QFormLayout* formLayout = new QFormLayout(this);
    nameInput = new QLineEdit();

    horInput = new QLineEdit();
    horInput->setValidator( new QIntValidator(1, 20, this) );
    horInput->setText("1");
    verInput = new QLineEdit();
    verInput->setValidator(new QIntValidator(1, 20, this));
    verInput->setText("1");
    physInput = new PhysInput(this, Rectangle(100, 100, 0, 0));

    formLayout->addRow(tr("Name :"), nameInput);
    formLayout->addRow(physInput);

    QPushButton* OKbutton = new QPushButton(tr("Add"));
    QPushButton* Canbutton = new QPushButton(tr("Cancel"));
    QDialogButtonBox* buttonBox = new QDialogButtonBox();
    buttonBox->addButton(OKbutton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(Canbutton, QDialogButtonBox::RejectRole);
    formLayout->addRow(buttonBox);
    QObject::connect(Canbutton, SIGNAL(clicked()), this, SLOT(hide()));
    QObject::connect(OKbutton, SIGNAL(clicked()), this, SLOT(add()));
}

void AddPlotDialog::add()
{
    QString name = nameInput->text();
    Plot& plot = plots_model->addPlot(name);

    plot.set_shape(new Rectangle(physInput->get_rect()));

    emit list_updated();
    hide();
}


PlotsWindow::PlotsWindow(PlotsModel *plots_model, QWidget* parent) :
  QDialog(parent), plots_model(plots_model)
{
    setModal(true);

    QVBoxLayout* plots_layout = new QVBoxLayout();
    plots_widget = new QListView(this);
    plots_widget->setModel(plots_model);
    plots_layout->addWidget(plots_widget);

    QPushButton* add_plot_btn = new QPushButton(tr("Add a plot"));
    AddPlotDialog* add_plot = new AddPlotDialog(plots_model);
    QObject::connect(add_plot_btn, SIGNAL(clicked()), add_plot, SLOT(show()));
    plots_layout->addWidget(add_plot_btn);

    del_plot_btn = new QPushButton(tr("Delete plot"));
    QObject::connect(del_plot_btn, SIGNAL(clicked()), this, SLOT(delete_plot()));
    plots_layout->addWidget(del_plot_btn);

    QPushButton* close_btn = new QPushButton(tr("Close"));
    plots_layout->addWidget(close_btn);
    QObject::connect(close_btn, SIGNAL(clicked()), this, SLOT(accept()));

    QObject::connect(add_plot, SIGNAL(list_updated()), plots_widget->model(), SIGNAL(layoutChanged()));
    QObject::connect(this, SIGNAL(plots_widget_layoutAboutToBeChanged()), plots_widget->model(), SIGNAL(layoutAboutToBeChanged()));
    QObject::connect(add_plot, SIGNAL(list_updated()), this, SIGNAL(timeline_need_update()));
    QObject::connect(this, SIGNAL(list_updated()), plots_widget->model(), SIGNAL(layoutChanged()));
    QObject::connect(this, SIGNAL(list_updated()), this, SIGNAL(timeline_need_update()));
    QObject::connect(plots_widget->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
                     this, SLOT(update_plot_data(const QModelIndex&, const QModelIndex&)));

    QVBoxLayout* notes_layout = new QVBoxLayout();
    notes_widget = new QTextEdit();
    notes_layout->addWidget(notes_widget);
    //update_notes();

    QVBoxLayout* details_layout = new QVBoxLayout();
    details_layout->addLayout(notes_layout);

    phys_widget = new PhysInput();
    details_layout->addWidget(phys_widget);

    QHBoxLayout* main_layout = new QHBoxLayout(this);
    main_layout->addLayout(plots_layout);
    main_layout->addLayout(details_layout);
    
    //update_btns_state();
}

Plot& PlotsWindow::selected_plot()
{
    int plot_index = plots_widget->selectionModel()->currentIndex().row();
    return plots_model->get_plots().index(plot_index);
}

void PlotsWindow::delete_plot()
{
    plots_widget->model()->removeRow(plots_widget->selectionModel()->currentIndex().row());
    emit timeline_need_update();
}

void PlotsWindow::update_plot_data(const QModelIndex& current_plot_mi, const QModelIndex& previous_plot_mi)
{
    int previous_plot_index = previous_plot_mi.row();
    int current_plot_index = current_plot_mi.row();
    update_notes(current_plot_index, previous_plot_index);
    update_phys(current_plot_index, previous_plot_index);
    update_del_btn(current_plot_index);
    emit timeline_need_update();
}

void PlotsWindow::update_del_btn(int current_plot_index)
{
    if (plots_model->get_plots().is_used(current_plot_index)) {
        del_plot_btn->setEnabled(false);
    } else {
        del_plot_btn->setEnabled(true);
    }
}

void PlotsWindow::update_notes(int current_plot_index, int previous_plot_index)
{
    //We save the note content of the previous plot
    if (previous_plot_index >= 0)
    {
        QString text = notes_widget->toPlainText();
        plots_model->get_plots().index(previous_plot_index).set_note(fromQString(text));
    }

    //and get the notes for the new plot
    if (current_plot_index >= 0)
    {
        QString text = toQString(plots_model->get_plots().index(current_plot_index).get_note());
        notes_widget->setPlainText(text);
        notes_widget->setDisabled(false);
    }
    else
    {
        notes_widget->setDisabled(true);
    }
}

void PlotsWindow::update_phys(int current_plot_index, int previous_plot_index)
{
    //We save the note content of the previous plot
    if (previous_plot_index >= 0)
    {
        plots_model->get_plots().index(previous_plot_index).set_shape(new Rectangle(phys_widget->get_rect()));
    }

    //and get the notes for the new plot
    if (current_plot_index >= 0)
    {
        Shape* shape = plots_model->get_plots().index(current_plot_index).get_shape();
        phys_widget->set_shape(shape);
        phys_widget->setDisabled(false);
    }
    else
    {
        phys_widget->setDisabled(true);
    }
}
