
#include "ComboBoxKey.h"
#include "../gui_utils.h"

ComboBoxKey::ComboBoxKey(QAbstractListModel* model)
{
    setModel(model);
}

void ComboBoxKey::setCurrentElem(QString key)
{
    for (int i=0; i < model()->rowCount(); i++)
    {
        QModelIndex index = model()->index(i, 1);  // column 1 contains keys
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
    if (model()->rowCount() > 0)
    {
        QModelIndex index = model()->index(currentIndex(), 1);  // column 1 contains keys
        return index.data().toString();
    }
    else
    {
        return QString();
    }
}
