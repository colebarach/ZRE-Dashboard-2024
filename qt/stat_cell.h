#ifndef STAT_CELL_H
#define STAT_CELL_H

// QT Libraries
#include <QWidget>
#include <QLabel>

class StatCell : public QFrame
{
    // Constructor / Destructor -----------------------------------------------------------------------------------------------

    public:

    StatCell(QWidget* parent);

    ~StatCell();

    // Protected Variables ----------------------------------------------------------------------------------------------------

    protected:

    QLabel* voltageStat;

    QPalette* framePalette;
};

#endif // STAT_CELL_H