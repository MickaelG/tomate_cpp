
#include <QWidget>
#include "dataset.h"

class TimelineWindow: public QWidget
{
    Q_OBJECT

    public:
        TimelineWindow(Dataset& data);
    private:
        Dataset& data;
};
