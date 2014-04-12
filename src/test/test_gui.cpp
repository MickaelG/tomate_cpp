
#include "dataset.h"
#include "gui_utils.h"
#include "gui_plants.h"
#include "PlantsModel.h"
#include <QApplication>
#include <QtTest/QTest>
using namespace QTest;

class TestClassDate: public QObject
{
    Q_OBJECT

private slots:
    void test_date()
    {
        QCOMPARE(QDate::currentDate(), toQDate(bg::day_clock::local_day()));
        QCOMPARE(fromQDate(QDate::currentDate()), bg::day_clock::local_day());

        QCOMPARE(QDate(), toQDate(bg::date()));
        QCOMPARE(fromQDate(QDate()), bg::date());
    }
};

class TestClassPlants: public QObject
{
    Q_OBJECT

private slots:
    void test_plant_note()
    {
        Crops crops;
        Plants plants(crops);
        Plant& plant1 = plants.add_plant("pl1", "plant1");
        Plant& plant2 = plants.add_plant("pl2", "plant2");
        plant1.set_note("Note for plant1");
        PlantsModel* plants_model =  new PlantsModel(plants);

        QCOMPARE(toQString(plant1.get_note()), QString("Note for plant1"));
        QCOMPARE(toQString(plant2.get_note()), QString(""));

        PlantsWindow* pw = new PlantsWindow(plants_model);
        pw->plants_widget->selectionModel()->setCurrentIndex(pw->plants_widget->model()->index(1,0), QItemSelectionModel::Rows);
        pw->plants_widget->selectionModel()->setCurrentIndex(pw->plants_widget->model()->index(0,0), QItemSelectionModel::Rows);

        QCOMPARE(toQString(plant1.get_note()), QString("Note for plant1"));
        QCOMPARE(toQString(plant2.get_note()), QString(""));
    }

    void test_plants_model()
    {
        Crops crops;
        Plants plants(crops);
        Plant& plant1 = plants.add_plant("pl1", "plant1");
        Plant& plant2 = plants.add_plant("pl2", "plant2");
        plant1.add_var("", "var1");
        plant1.add_var("", "var2");

        PlantsModel* plants_model =  new PlantsModel(plants);
        QModelIndex mi_pl0 = plants_model->index(0, 0);
        QCOMPARE(plants_model->data(mi_pl0).toString(), QString("plant1"));
        QModelIndex mi_pl1 = plants_model->index(1, 0);
        QCOMPARE(plants_model->data(mi_pl1).toString(), QString("plant2"));
        QModelIndex mi_var0 = plants_model->index(0, 0, mi_pl0);
        QCOMPARE(plants_model->data(mi_var0).toString(), QString("var1"));
        QModelIndex mi_var1 = plants_model->index(1, 0, mi_pl0);
        QCOMPARE(plants_model->data(mi_var1).toString(), QString("var2"));
        QCOMPARE(plants_model->rowCount(mi_pl0), 2);
        QCOMPARE(plants_model->rowCount(mi_pl1), 0);
        QCOMPARE(plants_model->rowCount(QModelIndex()), 2);
    }
};

//Because Q_OBJECT are not in the .h
#include "test_gui.moc"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TestClassDate test1;
    QTest::qExec(&test1, argc, argv);

    TestClassPlants test2;
    QTest::qExec(&test2, argc, argv);

    return 0;
}
