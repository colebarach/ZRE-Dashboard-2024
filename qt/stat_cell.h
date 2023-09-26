#ifndef STAT_CELL_H
#define STAT_CELL_H

// To do:
// - Converting doubles to the stat text could be better, it should handle clamping better and could be optimized. Look for
//   examples.
// - Temperature stat sucks, need a better way of rendering this data

// QT Libraries
#include <QWidget>
#include <QFrame>
#include <QLabel>

class StatCell : public QFrame
{
    // Constants --------------------------------------------------------------------------------------------------------------

    #define PADDING_INNER_FRAME     2    // Padding inside the inner frame, affects space between border and inner text
    #define SIZE_INNER_FRAME_BORDER 1    // Border thickness of the inner frame

    #define ID_MODE_OVERVIEW    0        // ID of overview mode, stat shows voltage, inner frame shows balancing, outer frame
                                         // shows temerature range.
    #define ID_MODE_VOLTAGE     1        // ID of voltage mode, stat shows voltage, inner and outer frames show balancing.
    #define ID_MODE_DELTA       2        // ID of delta mode, stat shows delta, inner and outer frames show balancing.
    #define ID_MODE_TEMPERATURE 3        // ID of temperature mode, stat shows temperature, inner and outer frames show
                                         // temperature range.

    // Constructor / Destructor -----------------------------------------------------------------------------------------------

    public:

    StatCell(QWidget* parent);

    ~StatCell();

    // Public Functions -------------------------------------------------------------------------------------------------------

    void set(double voltage, double temperature, double lowVoltage, bool isBalancing);

    // Protected Variables ----------------------------------------------------------------------------------------------------

    protected:

    int       mode;                      // Current mode of the widget, see mode IDs above for options.
    char      statBuffer[8];             // Buffer for stat data (3 int digits, 1 decimal point, 2 decimal digits, 1 terminator,
                                         // 1 new line)

    // Widgets
    QLabel*   innerStat;                 // Text for the stat to display (ex. voltage)
    QFrame*   innerFrame;                // Center frame, color based on mode (ex. isBalancing)

    // Palette
    QPalette* innerPalette;              // Palette for inner frame, used to modify color
    QPalette* outerPalette;              // Palette for outer frame, used to modify color

    // Protected Functions ----------------------------------------------------------------------------------------------------

    void updateVoltage();

    void updateTemperature();

    void updateBalancing();

    void updateDelta();

    static int getNthDigit(double value, int n);

    static void hsvLerp(double value, double h1, double s1, double v1, double h2, double s2, double v2, double* hOut, double* sOut, double* vOut);
};

#endif // STAT_CELL_H