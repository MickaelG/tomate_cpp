
#ifndef GUI_PLANTSMODEL_H
#define GUI_PLANTSMODEL_H

#include <QAbstractListModel>
#include <QComboBox>

#include "plants.h"
#include "keynames.h"
#include "plots.h"
#include "ListWidget.h"

class KeyNamesModel: public QAbstractListModel
{
    Q_OBJECT
    
public:
    KeyNamesModel(KeyNames& data_list, QWidget* parent = NULL);
    int rowCount(const QModelIndex& model_index) const;
    int columnCount(const QModelIndex& index) const;
    QVariant data(const QModelIndex& index, int role) const;
    
private:
    KeyNames& data_list;
};

class VarsModel: public QAbstractListModel
{
    Q_OBJECT
    
public:
    VarsModel(Plants& plants, ListWidget* plants_widget, QWidget* parent = NULL);
    VarsModel(Plants& plants, QComboBox* plants_widget, QWidget* parent = NULL);
    int rowCount(const QModelIndex& model_index) const;
    int columnCount(const QModelIndex& model_index) const;
    QVariant data(const QModelIndex& index, int role) const;
    
private:
    int get_plant_index() const;
    Plants& plants;
    ListWidget* plants_list_widget;
    QComboBox* plants_combo_widget;
    
};

class PlantsModel: public QAbstractListModel
{
    Q_OBJECT
    
public:
    PlantsModel(Plants& plants, QWidget* parent = NULL);
    int rowCount(const QModelIndex& model_index) const;
    int columnCount(const QModelIndex& index) const;
    QVariant data(const QModelIndex& index, int role) const;
    
private:
    Plants& plants;
};

class PlotsModel: public QAbstractListModel
{
    Q_OBJECT
    
public:
    PlotsModel(Plots& plots, QWidget* parent = NULL);
    int rowCount(const QModelIndex& model_index) const;
    int columnCount(const QModelIndex& index) const;
    QVariant data(const QModelIndex& index, int role) const;
    bool removeRows (int row, int count, const QModelIndex & parent = QModelIndex() );

private:
    Plots& plots;
};

class SubPlotsModel: public QAbstractListModel
{
    Q_OBJECT
    
public:
    SubPlotsModel(Plots& plots, ListWidget* plots_widget, QWidget* parent = NULL);
    SubPlotsModel(Plots& plots, QComboBox* plots_widget, QWidget* parent = NULL);
    int rowCount(const QModelIndex& model_index) const;
    int columnCount(const QModelIndex& model_index) const;
    QVariant data(const QModelIndex& index, int role) const;
    
private:
    int get_plot_index() const;
    Plots& plots;
    ListWidget* plots_list_widget;
    QComboBox* plots_combo_widget;
    
};

#endif //GUI_PLANTSMODEL_H

