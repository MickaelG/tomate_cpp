
#include "PlotsModel.h"

#include "gui_utils.h"

#include "plots.h"

using namespace std;


PlotsModel::PlotsModel(Plots& plots, QWidget* parent) :
    QAbstractListModel(parent), plots(plots)
{
}

Plots& PlotsModel::get_plots()
{
    return plots;
}

int PlotsModel::rowCount(const QModelIndex& parent) const
{
    return plots.size();
}

int PlotsModel::columnCount(const QModelIndex& index) const
{
    return 2;
}

QVariant PlotsModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            QString name = toQString(plots.index(index.row()).get_name());
            return QVariant(name);
        }
        else if (index.column() == 1)
        {
            QString key = toQString(plots.index(index.row()).get_key());
            return QVariant(key);
        }
    }
    return QVariant();
}

bool PlotsModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    bool retval = true;
    for (int row = 0; row < rows; ++row) {
        plots.delete_plot(position + row);
    }

    endRemoveRows();
    return retval;
}

Plot& PlotsModel::addPlot(QString plot_name)
{
   beginInsertRows(QModelIndex(), rowCount(), rowCount()+1);

   Plot& plot = plots.add_plot("", fromQString(plot_name));

   endInsertRows();

   return plot;
}

