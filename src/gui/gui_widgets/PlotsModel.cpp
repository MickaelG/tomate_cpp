
#include "PlotsModel.h"

#include "gui_utils.h"

#include "plots.h"

using namespace std;


PlotsModel::PlotsModel(Plots& plots, QWidget* parent) :
    QAbstractListModel(parent), plots(plots)
{
}

QModelIndex PlotsModel::parent(const QModelIndex &index) const
{
    if (index.internalId() == -1) // id == -1 is the plot list
    {
        return QModelIndex();
    }
    else // id >= 0 is the var list of the given plot index
    {
        return createIndex(index.internalId(), 0, -1);
    }

}

Plots& PlotsModel::get_plots()
{
    return plots;
}

const QModelIndex PlotsModel::plotIndex(int plot_index)
{
    return createIndex(plot_index, 0, -1);
}

int PlotsModel::rowCount(const QModelIndex& parent) const
{
    if (! parent.isValid())  // parent is invalid: we want the plot
    {
        return plots.size();
    }
    else
    {
        int plot_index =  parent.row();
        if (plot_index >= 0 && plot_index < plots.size())
        {
            Plots::iterator it = plots.begin();
            advance(it, plot_index);
            return it->get_subplots().size();
        }
        return 0;
    }
}

int PlotsModel::columnCount(const QModelIndex& index) const
{
    return 2;
}

QVariant PlotsModel::data(const QModelIndex& index, int role) const
{
    if (! index.parent().isValid())  // parent is invalid: we want the plot
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
    else
    {
        int plot_index =  index.parent().row();
        if (role == Qt::DisplayRole)
        {
            if (plot_index >= 0)
            {
                Plots::iterator it = plots.begin();
                advance(it, plot_index);
                list<Plot> subplots = it->get_subplots();
                list<Plot>::iterator itv = subplots.begin();
                advance(itv, index.row());
                if (index.column() == 0)
                {
                    //return QVariant(toQString(itv->get_name()));
                    return QVariant(toQString(itv->get_key()));
                }
                else if (index.column() == 1)
                {
                    return QVariant(toQString(itv->get_key()));
                }
            }
        }
        return QVariant();
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

QModelIndex PlotsModel::index(int row, int column, const QModelIndex& parent) const
{
    if (! parent.isValid())
    {
        return createIndex(row, column, -1);
    }
    else
    {
        return (createIndex(row, column, parent.row()));
    }
}

