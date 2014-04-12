
#ifndef GUI_COMBOBOXKEY_H
#define GUI_COMBOBOXKEY_H

#include <QComboBox>

class ComboBoxKey: public QComboBox
{
    //QtWidget to show a list of element and let the user chose one
    //(based on QComboBox).
    Q_OBJECT
public:
    ComboBoxKey(QWidget* parent=NULL);
    QString currentElem();
    void setCurrentElem(QString key);

public slots:
    void setVarRootModelIndex(int plant_index);

};

#endif //GUI_COMBOBOXKEY_H
