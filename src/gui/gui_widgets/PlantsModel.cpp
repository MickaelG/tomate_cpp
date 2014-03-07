
#include "PlantsModel.h"

#include "gui_utils.h"
using namespace std;

VarsModel::VarsModel(Plants& plants, ListWidget* plants_widget, QWidget* parent) :
    QAbstractListModel(parent), plants(plants),
    plants_list_widget(plants_widget), plants_combo_widget(0)
{
}

VarsModel::VarsModel(Plants& plants, QComboBox* plants_widget, QWidget* parent) :
    QAbstractListModel(parent), plants(plants),
    plants_list_widget(0), plants_combo_widget(plants_widget)
{
}


int VarsModel::get_plant_index() const
{
    if (plants_combo_widget)
    {
        return plants_combo_widget->currentIndex();
    }
    else if (plants_list_widget)
    {
        return plants_list_widget->index();
    }
    return -1;
}


int VarsModel::rowCount(const QModelIndex& model_index) const
{
    int plant_index = get_plant_index();
    if (plant_index >= 0)
    {
        Plants::iterator it = plants.begin();
        advance(it, plant_index);
        return it->get_vars().size();
    }
    return 0;
}

int VarsModel::columnCount(const QModelIndex& model_index) const
{
    return 2;
}


QVariant VarsModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            int plant_index = get_plant_index();
            if (plant_index > 0)
            {
                Plants::iterator it = plants.begin();
                advance(it, plant_index);
                Vars vars = it->get_vars();
                Vars::iterator itv = vars.begin();
                advance(itv, index.row());
                return QVariant(toQString(itv->get_name()));
            }
        }
        else if (index.column() == 1)
        {
            int plant_index = get_plant_index();
            if (plant_index > 0)
            {
                Plants::iterator it = plants.begin();
                advance(it, plant_index);
                Vars vars = it->get_vars();
                Vars::iterator itv = vars.begin();
                advance(itv, index.row());
                return QVariant(toQString(itv->get_key()));
            }
        }
    }
    return QVariant();
}

PlantsModel::PlantsModel(Plants& plants, QWidget* parent) :
    QAbstractListModel(parent), plants(plants)
{
}

int PlantsModel::rowCount(const QModelIndex& index) const
{
    return plants.size();
}

int PlantsModel::columnCount(const QModelIndex& index) const
{
    return 2;
}

QVariant PlantsModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            QString name = toQString(plants.index(index.row()).get_name());
            return QVariant(name);
        }
        else if (index.column() == 1)
        {
            QString key = toQString(plants.index(index.row()).get_key());
            return QVariant(key);
        }
    }
    return QVariant();
}

PlotsModel::PlotsModel(Plots& plots, QWidget* parent) :
    QAbstractListModel(parent), plots(plots)
{
}

int PlotsModel::rowCount(const QModelIndex& index) const
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
        Plot plot = plots.index(index.row());
        if (index.column() == 0)
        {
            QString name = toQString(plot.get_name());
            return QVariant(name);
        }
        else if (index.column() == 1)
        {
            QString key = toQString(plot.get_key());
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

SubPlotsModel::SubPlotsModel(Plots& plots, ListWidget* plots_widget, QWidget* parent) :
    QAbstractListModel(parent), plots(plots),
    plots_list_widget(plots_widget), plots_combo_widget(0)
{
}

SubPlotsModel::SubPlotsModel(Plots& plots, QComboBox* plots_widget, QWidget* parent) :
    QAbstractListModel(parent), plots(plots),
    plots_list_widget(0), plots_combo_widget(plots_widget)
{
}


int SubPlotsModel::get_plot_index() const
{
    if (plots_combo_widget)
    {
        return plots_combo_widget->currentIndex();
    }
    else if (plots_list_widget)
    {
        return plots_list_widget->index();
    }
    return -1;
}


int SubPlotsModel::rowCount(const QModelIndex& model_index) const
{
    int plot_index = get_plot_index();
    if (plot_index >= 0)
    {
        Plots::iterator it = plots.begin();
        advance(it, plot_index);
        return it->get_subplots().size();
    }
    return 0;
}

int SubPlotsModel::columnCount(const QModelIndex& model_index) const
{
    return 2;
}


QVariant SubPlotsModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        int plot_index = get_plot_index();
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


KeyNamesModel::KeyNamesModel(KeyNames& data_list, QWidget* parent) :
    QAbstractListModel(parent), data_list(data_list)
{
}

int KeyNamesModel::rowCount(const QModelIndex& index) const
{
    return data_list.size();
}

int KeyNamesModel::columnCount(const QModelIndex& index) const
{
    return 2;
}

QVariant KeyNamesModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            QString name = toQString(data_list.index(index.row()).get_name());
            return QVariant(name);
        }
        else if (index.column() == 1)
        {
            QString key = toQString(data_list.index(index.row()).get_key());
            return QVariant(key);
        }
    }
    return QVariant();
}
