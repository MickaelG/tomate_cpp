
#include "PlantsModel.h"

#include "gui_utils.h"

#include "plants.h"
using namespace std;


PlantsModel::PlantsModel(Plants& plants, QWidget* parent) :
    QAbstractListModel(parent), plants(plants)
{
}

QModelIndex PlantsModel::parent(const QModelIndex &index) const
{
    if (index.internalId() == -1) // id == -1 is the plant list
    {
        return QModelIndex();
    }
    else // id >= 0 is the var list of the given plant index
    {
        return createIndex(index.internalId(), 0, -1);
    }

}

Plants& PlantsModel::get_plants()
{
    return plants;
}

const QModelIndex PlantsModel::plantIndex(int plant_index)
{
    return createIndex(plant_index, 0, -1);
}

int PlantsModel::rowCount(const QModelIndex& parent) const
{
    if (! parent.isValid())  // parent is invalid: we want the plant
    {
        return plants.size();
    }
    else
    {
        int plant_index =  parent.row();
        if (plant_index >= 0 && plant_index < plants.size())
        {
            auto it = plants.begin();
            advance(it, plant_index);
            int var_size = it->get_vars().size();
            return var_size;
        }
        return 0;
    }
}

int PlantsModel::columnCount(const QModelIndex& parent) const
{
    return 2;
}

QVariant PlantsModel::data(const QModelIndex& index, int role) const
{
    if (! index.parent().isValid())  // parent is invalid: we want the plant
    {
        if (index.row() >= plants.size() || index.row() < 0) {
            return QVariant();
        }
        if (role == Qt::DisplayRole)
        {
            if (index.column() == 0)
            {
                QString name = toQString(plants[index.row()].get_name());
                return QVariant(name);
            }
            else if (index.column() == 1)
            {
                QString key = toQString(plants[index.row()].get_key());
                return QVariant(key);
            }
        }
        return QVariant();
    }
    else
    {
        int plant_index =  index.parent().row();
        if (plant_index >= plants.size() || plant_index < 0) {
            return QVariant();
        }
        if (role == Qt::DisplayRole)
        {
            if (index.column() == 0)
            {
                if (plant_index >= 0)
                {
                    auto it = plants.begin();
                    advance(it, plant_index);
                    const Vars& vars = it->get_vars();
                    auto itv = vars.begin();
                    if (index.row() > vars.size()-1) {
                        return QVariant(QString("ERROR"));
                    }
                    advance(itv, index.row());
                    return QVariant(toQString(itv->get_name()));
                }
            }
            else if (index.column() == 1)
            {
                if (plant_index > 0)
                {
                    auto it = plants.begin();
                    advance(it, plant_index);
                    const Vars& vars = it->get_vars();
                    auto itv = vars.begin();
                    advance(itv, index.row());
                    return QVariant(toQString(itv->get_key()));
                }
            }
        }
        return QVariant();
    }
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

void PlantsModel::addPlant(QString plant_name)
{
   beginInsertRows(QModelIndex(), rowCount(), rowCount()+1);

   plants.add("", fromQString(plant_name));

   endInsertRows();
}

void PlantsModel::addVar(const QModelIndex& plant_mi, QString var_name)
{
   beginInsertRows(plant_mi, rowCount(plant_mi), rowCount(plant_mi)+1);

   int plant_index = plant_mi.row();
   plants[plant_index].add_var("", fromQString(var_name));

   endInsertRows();
}

QModelIndex PlantsModel::index(int row, int column, const QModelIndex& parent) const
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
