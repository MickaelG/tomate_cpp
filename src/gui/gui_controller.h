
#include <QObject>
#include <QDate>
#include <memory>
#pragma once

class Crop;
class Dataset;

class CropSelectionController: public QObject
{
    Q_OBJECT

public:
    CropSelectionController(QObject* parent = nullptr) :
        QObject(parent), _selected_crop(nullptr)
    {
    }

    Crop* get_selected();

public slots:
    void select_crop(Crop *p_crop);

signals:
    void selection_changed(Crop* p_crop);

private:
    Crop* _selected_crop;
};

class DateController: public QObject
{
    Q_OBJECT

public:
    DateController(QObject* parent = nullptr) :
        QObject(parent),
        selected_date(QDate::currentDate())
    {
    }

    void set_date(QDate date)
    {
        bool year_changed = date.year() != selected_date.year();
        selected_date = date;
        emit date_changed(year_changed);
    }

    const QDate& get_date() const
    {
        return selected_date;
    }

signals:
    void date_changed(bool year_changed);

private:
    QDate selected_date;
    bool bWholeYear;
};

class DatasetModel: public QObject
{
   Q_OBJECT

public:
    DatasetModel(Dataset& dataset) :
        dataset(dataset)
    {
    }

    Dataset& get_dataset() { return dataset; }
    void emit_updated() { emit updated(); }

signals:
    void updated();

private:
    Dataset& dataset;
};

class DatasetController: public QObject
{
    Q_OBJECT

public:
    DatasetController(DatasetModel& model, CropSelectionController& selection_controller)
        : model(model), selection_controller(selection_controller)
    {
    }

    void remove_crop(Crop* crop);
    void update_current_crop(const Crop& new_crop);

private:
    DatasetModel& model;
    CropSelectionController& selection_controller;
};
