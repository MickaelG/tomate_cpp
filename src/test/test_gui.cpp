
#include "dataset.h"
#include "gui_utils.h"
#include "gui_plants.h"
#include "ListWidget.h"
#include <QApplication>
#include <QtTest/QTest>

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

class TestClassGui: public QObject
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

        QCOMPARE(toQString(plant1.get_note()), QString("Note for plant1"));
        QCOMPARE(toQString(plant2.get_note()), QString(""));

        PlantsWindow* pw = new PlantsWindow(plants);
        pw->plants_widget->selectionModel()->setCurrentIndex(pw->plants_widget->model()->index(1,0), QItemSelectionModel::Rows);
        pw->plants_widget->selectionModel()->setCurrentIndex(pw->plants_widget->model()->index(0,0), QItemSelectionModel::Rows);

        QCOMPARE(toQString(plant1.get_note()), QString("Note for plant1"));
        QCOMPARE(toQString(plant2.get_note()), QString(""));
    }
};

//Because Q_OBJECT are not in the .h
#include "test_gui.moc"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TestClassDate test1;
    QTest::qExec(&test1, argc, argv);

    TestClassGui test2;
    QTest::qExec(&test2, argc, argv);

    return 0;
}
