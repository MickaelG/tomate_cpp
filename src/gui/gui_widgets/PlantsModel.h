
#ifndef GUI_PLANTSMODEL_H
#define GUI_PLANTSMODEL_H

#include <QAbstractListModel>
#include <QComboBox>

class Plants;
class Plant;


class PlantsModel: public QAbstractListModel
{
    Q_OBJECT
    
public:
    PlantsModel(Plants& plants, QWidget* parent = NULL);
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    bool removeRows (int row, int count, const QModelIndex & parent = QModelIndex() );
    Plants& get_plants();
    void addPlant(QString plant_name);
    void addVar(const QModelIndex& plant_mi, QString var_name);
    const QModelIndex plantIndex(int plant_index);
    QModelIndex index (int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    QModelIndex parent(const QModelIndex &index) const;
    int GetSpeciesRowIndex(const Plant& plant) const;
    int GetVarietyRowIndex(const Plant& plant) const;
    Plant* GetPlant(int plant_index, int var_index) const;

private:
    Plants& plants;
};

#endif //GUI_PLANTSMODEL_H

