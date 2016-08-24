
#include <vector>
using namespace std;

class Plants;
class PlantSpecies;
class PlantsModel;

#include <QWidget>
#include <QLayout>
#include <QWidget>
#include <QPushButton>
#include <QListView>
#include <QTextEdit>
#include <QLineEdit>
#include <QColorDialog>
#include <QPushButton>

class AddDialog: public QDialog
{
    Q_OBJECT

public:
   AddDialog(PlantsModel *plants_model, QWidget* parent=NULL);

signals:
    void list_updated();

private slots:
    void add();
    virtual void add_elem(QString name) = 0;
    
protected:
    PlantsModel* plants_model;
    QLineEdit* nameInput;
};

class PlantsWindow: public QDialog
{
    Q_OBJECT

public:
    PlantsWindow(PlantsModel *plants_model, QWidget* parent = NULL);
    PlantSpecies& selected_plant();

signals:
    void timeline_need_update();
    
private slots:
    void set_color();
    void delete_plant();
    void update_plant_data(const QModelIndex& current_plant_mi, const QModelIndex& previous_plant_mi);
    void update_btns_state();
    void update_del_btn(int current_plant_index);
    void update_notes(int current_plant_index, int previous_plant_index);

public:
    QListView* plants_widget;

private:
    PlantsModel* plants_model;
    QPushButton* color_btn;
    QTextEdit* notes_widget;
    QPushButton* add_var_btn;
    QListView* var_widget;
    QColorDialog* color_dialog;
    QPushButton* del_plant_btn;
};
