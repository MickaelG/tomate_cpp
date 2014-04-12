
#ifndef GUI_PLOTSMODEL_H
#define GUI_PLOTSMODEL_H

#include <QAbstractListModel>
#include <QComboBox>

class Plots;
class Plot;

class PlotsModel: public QAbstractListModel
{
    Q_OBJECT
    
public:
    PlotsModel(Plots& plots, QWidget* parent = NULL);
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    bool removeRows (int row, int count, const QModelIndex & parent = QModelIndex() );
    Plots& get_plots();
    Plot& addPlot(QString plot_name);
    const QModelIndex plotIndex(int plot_index);

private:
    Plots& plots;
};

#endif //GUI_PLOTSMODEL_H

