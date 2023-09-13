#ifndef VIEW_BMS_H
#define VIEW_BMS_H

// BMS View -------------------------------------------------------------------------------------------------------------------
//
// Description: BMS monitoring tool
//
// Created: 23.09.04
// Updated: 23.09.04

// Libraries ------------------------------------------------------------------------------------------------------------------

// Includes
#include "view.h"
#include "stat_cell.h"

// QT Libraries
#include <QWidget>

// Database Keys --------------------------------------------------------------------------------------------------------------

#define CELL_VOLTAGE_N "Voltage_Cell_%lu" // Double (0 <= n < SEGMENT_COUNT * CELLS_PER_SEGMENT)

// Classes --------------------------------------------------------------------------------------------------------------------

// View UI forward declaration
// - This namespace and class are generated at compile-time using the form file
namespace Ui
{
    class ViewBms;
}

class ViewBms : public View
{
    Q_OBJECT // QT object boilerplate
    
    // Constants --------------------------------------------------------------------------------------------------------------

    #define SEGMENT_COUNT     5
    #define CELLS_PER_SEGMENT 18

    // Constructor / Destructor -----------------------------------------------------------------------------------------------

    public:

    ViewBms(QWidget* parent_, MainWindow* mainWindow_, Network::Database* database_);

    ~ViewBms();

    // Public Funtions --------------------------------------------------------------------------------------------------------

    void update() override;

    // Slots ------------------------------------------------------------------------------------------------------------------

    private slots:

    void handleButtonMenu();

    // Protected Variables ----------------------------------------------------------------------------------------------------

    protected:

    Ui::ViewBms* ui;

    StatCell*** cellStats;

    double*** cellVoltages;
    bool***   cellBalangings;
};

#endif // VIEW_BMS_H