
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
    if (index.row() >= plots.size() || index.row() < 0) {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            QString name = toQString(plots[index.row()].get_name());
            return QVariant(name);
        }
    }
    return QVariant();
}

bool PlotsModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    bool retval = true;
    for (int row = 0; row < rows; ++row) {
        plots.remove(position + row);
    }

    endRemoveRows();
    return retval;
}

Plot& PlotsModel::addPlot(QString plot_name)
{
   beginInsertRows(QModelIndex(), rowCount(), rowCount()+1);

   Plot& plot = plots.add(fromQString(plot_name));

   endInsertRows();

   return plot;
}

int PlotsModel::GetRowIndex(const Plot &plot) const
{
    for (int plot_index = 0; plot_index < plots.size(); ++plot_index) {
        if (plots[plot_index] == plot) {
            return plot_index;
        }
    }
    return -1;
}

Plot* PlotsModel::GetPlot(int plot_index) const
{
    if (plot_index < 0 || plot_index >= plots.size()) {
        return nullptr;
    }
    return &plots[plot_index];
}
