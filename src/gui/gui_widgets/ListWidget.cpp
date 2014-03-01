
#include "ListWidget.h"

ListModel::ListModel(vector<QString> elem_list, QWidget* parent) :
    QAbstractListModel(parent)
{
    this->elem_list = elem_list;
}

int ListModel::rowCount(const QModelIndex& index) const
{
    return elem_list.size();
}

QVariant ListModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            return QVariant(elem_list[index.row()]);
        }
    }
    return QVariant();
}


ListWidget::ListWidget(vector<QString> elem_list, QWidget* parent) :
    QListView(parent)
{
    ListModel* model = new ListModel(elem_list);
    this->setModel(model);
}

ListWidget::ListWidget(QAbstractListModel* model, QWidget* parent) :
    QListView(parent)
{
    this->setModel(model);
}

int ListWidget::index()
{
    int result = selectionModel()->currentIndex().row();
    if (result < 0)
    {
        return -1;
    }
    return result;
}
