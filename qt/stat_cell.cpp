// Header
#include "stat_cell.h"

// Includes
#include "log.h"

// C Standard Libraries
#include "math.h"

// QT Libraries
#include <QLayout>
#include <QGridLayout>
#include <QColor>

StatCell::StatCell(QWidget* parent) : QFrame(parent)
{
    // Create widgets
    innerFrame = new QFrame(this);
    innerStat  = new QLabel(innerFrame);

    // Set text alignment
    innerStat->setAlignment(Qt::AlignCenter);

    // Create outer frame palette
    outerPalette = new QPalette(palette());
    outerPalette->setColor(QPalette::ColorRole::Window, Qt::darkGray);
    setAutoFillBackground(true);
    setPalette(*outerPalette);

    // Create inner frame palette
    innerPalette = new QPalette(palette());
    innerFrame->setFrameShape(QFrame::Box);
    innerPalette->setColor(QPalette::ColorRole::Window, Qt::darkGray);
    innerPalette->setColor(QPalette::ColorRole::Mid,    Qt::white);
    innerFrame->setAutoFillBackground(true);
    innerFrame->setPalette(*innerPalette);
    
    // Apply outer layout
    QGridLayout* outerLayout = new QGridLayout();
    outerLayout->setContentsMargins(PADDING_INNER_FRAME, PADDING_INNER_FRAME, PADDING_INNER_FRAME, PADDING_INNER_FRAME);
    outerLayout->addWidget(innerFrame);
    setLayout(outerLayout);

    // Apply inner layout
    QGridLayout* innerLayout = new QGridLayout();
    innerLayout->setContentsMargins(PADDING_INNER_FRAME, PADDING_INNER_FRAME, PADDING_INNER_FRAME, PADDING_INNER_FRAME);
    innerLayout->addWidget(innerStat);
    innerFrame->setLayout(innerLayout);

    // Create colors
    colorBalancingOn  = new QColor(COLOR_CELL_BALANCING_ON);
    colorBalancingOff = new QColor(COLOR_CELL_BALANCING_OFF);

    // Set default mode
    mode = ID_MODE_OVERVIEW;
}

StatCell::~StatCell()
{
    // TODO: delete other stuff
    // TODO: deleting these causes seg fault?
    // delete innerFrame;
    // delete innerStat;
}

void StatCell::set(double voltage, double temperature, double lowVoltage, bool isBalancing)
{
    if(mode == ID_MODE_OVERVIEW)
    {
        // Render Border ------------------------------------------------------------------------------------------------------
        innerFrame->setLineWidth(SIZE_INNER_FRAME_BORDER);

        // Render Inner Frame -------------------------------------------------------------------------------------------------
        if(isBalancing)
        {
            innerPalette->setColor(QPalette::ColorRole::Window, *colorBalancingOn);
            innerFrame->setPalette(*innerPalette);
        }
        else
        {
            innerPalette->setColor(QPalette::ColorRole::Window, *colorBalancingOff);
            innerFrame->setPalette(*innerPalette);
        }

        // Render Outer Frame -------------------------------------------------------------------------------------------------
        
        // TODO

        // Render Stat --------------------------------------------------------------------------------------------------------

        // Prevent underflow
        if(voltage < 0)
        {
            LOG_WARN("Clamping voltage less than 0...");
            voltage = 0;
        }

        // Prevent overflow
        if(voltage > 9.99)
        {
            LOG_WARN("Clamping voltage greater than 9.99...");
            voltage = 9.99;
        }

        // Round to 2 decimal places
        voltage = round(voltage * 100) / 100.0;

        double voltageInt;
        double voltageFrac = modf(voltage, &voltageInt);
        snprintf(statBuffer, 6, "%1u.\n%02u", static_cast<unsigned int>(voltageInt), static_cast<unsigned int>(voltageFrac * 100));
        statBuffer[5] = '\0';

        innerStat->setText(statBuffer);
    }
    else if(mode == ID_MODE_VOLTAGE)
    {
        // Disable Border -----------------------------------------------------------------------------------------------------
        innerFrame->setLineWidth(0);

        // Render Inner / Outer Frame -----------------------------------------------------------------------------------------
        if(isBalancing)
        {
            innerPalette->setColor(QPalette::ColorRole::Window, *colorBalancingOn);
            outerPalette->setColor(QPalette::ColorRole::Window, *colorBalancingOn);
            innerFrame->setPalette(*innerPalette);
            setPalette(*outerPalette);
        }
        else
        {
            innerPalette->setColor(QPalette::ColorRole::Window, *colorBalancingOff);
            outerPalette->setColor(QPalette::ColorRole::Window, *colorBalancingOff);
            innerFrame->setPalette(*innerPalette);
            setPalette(*outerPalette);
        }

        // Render stat --------------------------------------------------------------------------------------------------------

        // TODO
    }
    else if(mode == ID_MODE_DELTA)
    {
        // Disable Border -----------------------------------------------------------------------------------------------------
        innerFrame->setLineWidth(0);

        // Render Inner / Outer Frame -----------------------------------------------------------------------------------------
        if(isBalancing)
        {
            innerPalette->setColor(QPalette::ColorRole::Window, *colorBalancingOn);
            outerPalette->setColor(QPalette::ColorRole::Window, *colorBalancingOn);
            innerFrame->setPalette(*innerPalette);
            setPalette(*outerPalette);
        }
        else
        {
            innerPalette->setColor(QPalette::ColorRole::Window, *colorBalancingOff);
            outerPalette->setColor(QPalette::ColorRole::Window, *colorBalancingOff);
            innerFrame->setPalette(*innerPalette);
            setPalette(*outerPalette);
        }

        // Render Stat --------------------------------------------------------------------------------------------------------

        // Calculate delta
        double delta = voltage - lowVoltage;

        // Prevent underflow
        if(delta < 0)
        {
            LOG_WARN("Clamping delta less than 0...");
            delta = 0;
        }

        // Prevent overflow
        if(delta > 9.99)
        {
            LOG_WARN("Clamping delta greater than 9.99...");
            delta = 9.99;
        }

        // Round to 2 decimal places
        delta = round(delta * 100) / 100.0;

        // TODO
        statBuffer[5] = '\0';

        innerStat->setText(statBuffer);
    }
    else if(mode == ID_MODE_TEMPERATURE)
    {
        // Don't render border
        innerFrame->setLineWidth(0);

        // Temperature
        double h1 = 120;
        double s1 = 255;
        double v1 = 255;
        double h2 = 0;
        double s2 = 255;
        double v2 = 255;

        StatCell::hsvLerp(temperature / 100.0, h1, s1, v1, h2, s2, v2, &h1, &s1, &v1);

        QColor tempColor = QColor::fromHsv(h1, s1, v1);

        innerPalette->setColor(QPalette::ColorRole::Window, tempColor);
        innerFrame->setPalette(*innerPalette);

        // Prevent underflow (TODO: Are negative temperatures important?)
        if(temperature < 0)
        {
            LOG_WARN("Clamping temperature less than 0...");
            temperature = 0;
        }

        // Prevent overflow
        if(temperature > 999.99)
        {
            LOG_WARN("Clamping temperature greater than 999.99...");
            temperature = 999.99;
        }

        // Round to 2 decimal place
        temperature = round(temperature * 100) / 100.0;

        // Write first digit
        // - The " + '0' " adds the temperature's value to the start of the numerals in ASCII, this converts an integer to a
        //   single digit
        statBuffer[0] = static_cast<int>(trunc(temperature / 100.0)) + '0';
        if(statBuffer[0] == '0') statBuffer[0] = ' ';

        // Write second digit
        statBuffer[1] = static_cast<int>(trunc(temperature / 10.0)) - static_cast<int>(trunc(temperature / 100.0)) * 10.0 + '0';
        if(statBuffer[1] == '0' && statBuffer[0] == ' ') statBuffer[1] = ' ';

        // Write third digit
        statBuffer[2] = static_cast<int>(trunc(temperature)) - (static_cast<int>(trunc(temperature / 10.0)) - static_cast<int>(trunc(temperature / 100.0)) * 10.0) * 10.0 + '0';

        // Insert new line
        statBuffer[3] = '\n';

        // Insert decimal point
        statBuffer[4] = '.';

        // Insert first digit
        // - See above for char addition
        statBuffer[5] = StatCell::getNthDigit(temperature, 1) + '0';
        
        // Insert second digit
        // - See above for char addition
        statBuffer[6] = StatCell::getNthDigit(temperature, 2) + '0';

        // Terminate string
        statBuffer[7] = '\0';

        innerStat->setText(statBuffer);
    }
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

void StatCell::hsvLerp(double value, double h1, double s1, double v1, double h2, double s2, double v2, double* hOut, double* sOut, double* vOut)
{
    *hOut = value * (h2 - h1) + h1;
    *sOut = value * (s2 - s1) + s1;
    *vOut = value * (v2 - v1) + v1; 
}