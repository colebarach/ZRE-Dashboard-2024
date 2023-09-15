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

    char dataBuffer[6]; // 1 digit, 1 decimal point, 2 digits precision

    // Protected Functions ----------------------------------------------------------------------------------------------------

    static int getNthDigit(double value, int n);
};

#endif // STAT_CELL_H