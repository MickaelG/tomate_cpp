
#include <QWidget>
#include "dataset.h"

class PlantsWindow: public QWidget
{
    Q_OBJECT

    public:
        PlantsWindow(Plants& plants);
};
