
class Plot;
class Plots;
class ListWidget;
class PlantModel;
class PhysInput;
class SubdWidget;

#include <QDialog>
#include <QWidget>
#include <QLayout>
#include <QWidget>
#include <QPushButton>
#include <QListView>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>

class AddPlotDialog: public QDialog
{
    Q_OBJECT

public:
   AddPlotDialog(Plots& plots, QWidget* parent=NULL);

signals:
    void list_updated();

private slots:
    void add();
    
private:
    Plots& plots;
    QLineEdit* nameInput;
    PhysInput* physInput;
    QLineEdit* verInput;
    QLineEdit* horInput;
};

class PlotsWindow: public QDialog
{
    Q_OBJECT

public:
    PlotsWindow(Plots& plots, QWidget* parent = NULL);
    Plot& selected_plot();

signals:
    void timeline_need_update();
    void list_updated();
    void plots_widget_layoutAboutToBeChanged();

private slots:
    void delete_plot();
    void update_plot_data(const QModelIndex& current_plot_mi, const QModelIndex& previous_plot_mi);
    void update_notes(int current_plot_index, int previous_plot_index);
    void update_phys(int current_plot_index, int previous_plot_index);
    void update_del_btn(int current_plot_index);

private:
    Plots& plots;
    QTextEdit* notes_widget;
    ListWidget* plots_widget;
    PhysInput* phys_widget;
    QPushButton* del_plot_btn;
};
