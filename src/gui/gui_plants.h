
#include <vector>
using namespace std;

class Plants;
class Plant;
class ListWidget;
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
   AddDialog(Plants& plants, QWidget* parent=NULL);

signals:
    void list_updated();

private slots:
    void add();
    virtual void add_elem(QString name) = 0;
    
protected:
    Plants& plants;
private:
    QLineEdit* keyInput;
    QLineEdit* nameInput;
};

class PlantsWindow: public QDialog
{
    Q_OBJECT

public:
    PlantsWindow(Plants& plants, QWidget* parent = NULL);
    Plant& selected_plant();

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
    ListWidget* plants_widget;

private:
    vector<QString> plants_str_list();
    Plants& plants;
    QPushButton* color_btn;
    QTextEdit* notes_widget;
    QPushButton* add_var_btn;
    QListView* var_widget;
    QColorDialog* color_dialog;
    QPushButton* del_plant_btn;
};
