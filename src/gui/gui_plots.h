
class Plot;
class Plots;
class PlotsModel;
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
   AddPlotDialog(PlotsModel *plots_model, QWidget* parent=NULL);

signals:
    void list_updated();

private slots:
    void add();
    
private:
    PlotsModel* plots_model;
    QLineEdit* nameInput;
    PhysInput* physInput;
    QLineEdit* verInput;
    QLineEdit* horInput;
};

class PlotsWindow: public QDialog
{
    Q_OBJECT

public:
    PlotsWindow(PlotsModel* plots_model, QWidget* parent = NULL);
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
    PlotsModel* plots_model;
    QTextEdit* notes_widget;
    QListView* plots_widget;
    PhysInput* phys_widget;
    QPushButton* del_plot_btn;
};
