// Header
#include "stat_cell.h"

// Includes
#include "log.h"

// C Standard Libraries
#include "math.h"

// QT Libraries
#include <QLayout>
#include <QGridLayout>

StatCell::StatCell(QWidget* parent) : QFrame(parent)
{
    framePalette = new QPalette(palette());
    framePalette->setColor(QPalette::ColorRole::Window, Qt::red);
    setAutoFillBackground(true);
    setPalette(*framePalette);

    // Create voltage label
    voltageStat = new QLabel(this);
    
    // Apply layout
    QGridLayout* layout_ = new QGridLayout();
    layout_->addWidget(voltageStat);
    setLayout(layout_);
}

StatCell::~StatCell()
{
    delete voltageStat;
}

void StatCell::setVoltage(double voltage)
{
    // Round to 2 decimal places
    voltage = round(voltage * 100) / 100.0;

    // Get first digit of number
    if(voltage >= 9)
    {
        // Prevent overflow
        dataBuffer[0] = '9';
    }
    else if(voltage <= 0)
    {
        // Prevent underflow
        dataBuffer[0] = '0';
    }
    else
    {
        // Write first digit
        // - The " + '0' " adds the voltage's value to the start of the numerals in ASCII, this converts an integer to a
        //   single digit
        dataBuffer[0] = static_cast<int>(trunc(voltage)) + '0';
    }

    // Insert decimal point
    dataBuffer[1] = '.';

    // Insert new line
    dataBuffer[2] = '\n';

    // Insert first digit
    // - See above for char addition
    dataBuffer[3] = StatCell::getNthDigit(voltage, 1) + '0';
    
    // Insert second digit
    // - See above for char addition
    dataBuffer[4] = StatCell::getNthDigit(voltage, 2) + '0';

    // Terminate string
    dataBuffer[5] = '\0';

    voltageStat->setText(dataBuffer);
}

int StatCell::getNthDigit(double value, int n)
{
    int tenToTheN = 1;
    for(int x = 0; x < n; ++x)
    {
        tenToTheN *= 10;
    }

    int upperDigits = static_cast<int>(trunc(value * tenToTheN));

    return upperDigits % 10;
}