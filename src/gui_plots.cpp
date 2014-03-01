
#include "gui_plots.h"
#include "gui_utils.h"

#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>


AddPlotDialog::AddPlotDialog(Plots& plots, QWidget* parent) :
  plots(plots), QDialog(parent)
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
    physInput = new PhysInput(Rectangle(100, 100, 0, 0));

    formLayout->addRow(tr("Name :"), nameInput);
    formLayout->addRow(physInput);
    formLayout->addRow(tr("Horizontal subdivisions :"), horInput);
    formLayout->addRow(tr("Vertical subdivisons :"), verInput);

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
    Plot& plot = plots.add_plot("", fromQString(name));

    plot.set_rect(physInput->get_rect());

    int nb_hor = horInput->text().toInt();
    int nb_ver = verInput->text().toInt();
    plot.create_subplots(nb_hor, nb_ver);

    emit list_updated();
    hide();
}


PlotsWindow::PlotsWindow(Plots& plots, QWidget* parent) :
  QWidget(parent), plots(plots)
{
    QVBoxLayout* plots_layout = new QVBoxLayout();
    plots_widget = new ListWidget(new PlotsModel(plots));
    plots_layout->addWidget(plots_widget);

    QPushButton* add_plot_btn = new QPushButton(tr("Add a plot"));
    AddPlotDialog* add_plot = new AddPlotDialog(plots);
    QObject::connect(add_plot_btn, SIGNAL(clicked()), add_plot, SLOT(show()));
    plots_layout->addWidget(add_plot_btn);

    QPushButton* del_plot_btn = new QPushButton(tr("Delete plot"));
    QObject::connect(del_plot_btn, SIGNAL(clicked()), this, SLOT(delete_plot()));
    plots_layout->addWidget(del_plot_btn);

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

    subd_widget = new SubdWidget();
    details_layout->addWidget(subd_widget);

    QHBoxLayout* main_layout = new QHBoxLayout(this);
    main_layout->addLayout(plots_layout);
    main_layout->addLayout(details_layout);
    
    //update_btns_state();
}

Plot& PlotsWindow::selected_plot()
{
    int plot_index = plots_widget->selectionModel()->currentIndex().row();
    return plots.index(plot_index);
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
    update_subd(current_plot_index, previous_plot_index);
    emit timeline_need_update();
}

void PlotsWindow::update_notes(int current_plot_index, int previous_plot_index)
{
    //We save the note content of the previous plot
    if (previous_plot_index >= 0)
    {
        QString text = notes_widget->toPlainText();
        plots.index(previous_plot_index).set_note(fromQString(text));
    }

    //and get the notes for the new plot
    if (current_plot_index >= 0)
    {
        QString text = toQString(plots.index(current_plot_index).get_note());
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
        plots.index(previous_plot_index).set_rect(phys_widget->get_rect());
    }

    //and get the notes for the new plot
    if (current_plot_index >= 0)
    {
        phys_widget->set_rect(plots.index(current_plot_index).get_rect());
        phys_widget->setDisabled(false);
    }
    else
    {
        phys_widget->setDisabled(true);
    }
}

void PlotsWindow::update_subd(int current_plot_index, int previous_plot_index)
{
    //TODO: save changes on subplots
    //if (previous_plot_index >= 0)
    //{
    //    QVector<Rectangle> all_rects = subd_widget->get_rects();
    //    for (int rect_index = 0; rect_index < all_rects.size(); rect_index++)
    //    {
    //        plots.index(previous_plot_index).get_subplots().index(rect_index).set_rect(all_rects[rect_index]);
    //    }
    //}

    if (current_plot_index >= 0)
    {
        QVector<Rectangle> all_rects;
        for (Plot plot: plots.index(current_plot_index).get_subplots())
        {
            all_rects.push_back(plot.get_rect());
        }
        subd_widget->set_rects(all_rects);
        subd_widget->setDisabled(false);
    }
    else
    {
        subd_widget->set_rects();
        subd_widget->setDisabled(true);
    }
}
