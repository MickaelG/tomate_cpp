
#ifndef LISTWIDGET
#define LISTWIDGET

#include <QAbstractListModel>
#include <QListView>
#include <QString>

#include <vector>
using namespace std;

class ListModel: public QAbstractListModel
{
    Q_OBJECT

public:
    ListModel(vector<QString> elem_list, QWidget* parent = 0);
    int rowCount(const QModelIndex& index) const;
    QVariant data(const QModelIndex& index, int role) const;

private:
    vector<QString> elem_list;
};

class ListWidget: public QListView
{
    Q_OBJECT

public:
    ListWidget(vector<QString> elem_list = vector<QString>(), QWidget* parent = 0);
    ListWidget(QAbstractListModel* model, QWidget* parent = 0);
    int index();
};

#endif
