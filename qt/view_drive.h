#ifndef VIEW_DRIVE_H
#define VIEW_DRIVE_H

// Drive View -----------------------------------------------------------------------------------------------------------------
//
// Description: A view of the GUI used while driving.
//
// Created: 23.08.22
// Updated: 23.08.23
//
// To do:
// - Database signedness is important, currently being ignored.

// Libraries ------------------------------------------------------------------------------------------------------------------

// Includes
#include "view.h"
#include "strata_bar.h"

// QT Libraries
#include <QWidget>

// Database Keys --------------------------------------------------------------------------------------------------------------

#define MOTOR_SPEED      "Motor_Speed"                   // Integer
#define MOTOR_MPH        "Motor_Speed"                   // Integer
#define STATE_OF_CHARGE  "State_of_Charge"               // Integer
#define TEMP_ACC_MAX     "Temperature_BMS_Max"           // Integer
#define TEMP_INV_MAX     "Temperature_Inverter_Module_A" // Integer
#define TEMP_MOTOR_MAX   "Temperature_Motor"             // Integer
#define THROTTLE_PERCENT "APPS_1_Percent"                // Integer
#define BRAKE_PERCENT    "Brake_1_Percent"               // Integer
#define TORQUE_PERCENT   "Torque_Config_Limit"           // Integer
#define REGEN_PERCENT    "Torque_Config_Limit_Regen"     // Integer

// Classes --------------------------------------------------------------------------------------------------------------------

// View UI forward declaration
// - This namespace and class are generated at compile-time using the form file
namespace Ui
{
    class ViewDrive;
}

class ViewDrive : public View
{
    Q_OBJECT // QT object boilerplate
    
    // Constructor / Destructor -----------------------------------------------------------------------------------------------

    public:

    ViewDrive(QWidget* parent_, MainWindow* mainWindow_, Network::Database* database_);

    ~ViewDrive();

    // Public Funtions --------------------------------------------------------------------------------------------------------

    void update() override;

    // Slots ------------------------------------------------------------------------------------------------------------------

    private slots:

    void handleButtonSpeed();

    void handleButtonEndurance();

    void handleButtonMenu();

    // Protected Variables ----------------------------------------------------------------------------------------------------

    protected:

    Ui::ViewDrive* ui;

    StrataBar* rpmBar;

    int* motorSpeed;
    int* motorMph;
    int* stateOfCharge;
    int* tempAccumulatorMax;
    int* tempInverterMax;
    int* tempMotorMax;
    int* throttlePercent;
    int* brakePercent;
    int* torquePercent;
    int* regenPercent;
};

#endif // VIEW_DRIVE_H