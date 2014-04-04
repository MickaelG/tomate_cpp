
#include "PlantsModel.h"

#include "gui_utils.h"

#include "plants.h"
#include "keynames.h"
#include "plots.h"
#include "plot.h"
#include "ListWidget.h"

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
            if (plant_index >= 0)
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

bool PlantsModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    bool retval = true;
    for (int row = 0; row < rows; ++row) {
        plants.delete_plant(position + row);
    }

    endRemoveRows();
    return retval;
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
