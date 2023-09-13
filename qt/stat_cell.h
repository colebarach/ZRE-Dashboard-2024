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

    // Public Functions -------------------------------------------------------------------------------------------------------

    void setVoltage(double voltage);

    // Protected Variables ----------------------------------------------------------------------------------------------------

    protected:

    QLabel* voltageStat;

    QPalette* framePalette;

    QString* voltageText;

    // Protected Functions ----------------------------------------------------------------------------------------------------

    static int getNthDigit(double value, int n);
};

#endif // STAT_CELL_H