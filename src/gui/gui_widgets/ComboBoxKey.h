
#ifndef GUI_COMBOBOXKEY_H
#define GUI_COMBOBOXKEY_H

#include <QComboBox>
#include "PlantsModel.h"

class ComboBoxKey: public QComboBox
{
    //QtWidget to show a list of element and let the user chose one
    //(based on QComboBox).
    Q_OBJECT
public:
    ComboBoxKey(QWidget* parent=NULL);
    QString currentElem();
    void setCurrentElem(QString key);

};

#endif //GUI_COMBOBOXKEY_H
