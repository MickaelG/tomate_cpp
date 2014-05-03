
#include "ComboBoxKey.h"
#include "gui_utils.h"

ComboBoxKey::ComboBoxKey(QWidget *parent) :
    QComboBox(parent)
{
}

void ComboBoxKey::setCurrentElem(QString key)
{
    for (int i=0; i < model()->rowCount(rootModelIndex()); i++)
    {
        QModelIndex index = model()->index(i, 1, rootModelIndex());  // column 1 contains keys
        if(index.data().toString() == key)
        {
            setCurrentIndex(i);
            return;
        }
    }
    return;
}

QString ComboBoxKey::currentElem()
{
    if (model()->rowCount(rootModelIndex()) > 0)
    {
        QModelIndex index = model()->index(currentIndex(), 1, rootModelIndex());  // column 1 contains keys
        return index.data().toString();
    }
    else
    {
        return QString();
    }
}

void ComboBoxKey::setVarRootModelIndex(int plant_index)
{
    setRootModelIndex(model()->index(plant_index, 0));
    if (count() == 0)
    {
        setCurrentIndex(-1);
    }
    else
    {
        setCurrentIndex(0);
    }
}
