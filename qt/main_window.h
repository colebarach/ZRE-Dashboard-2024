#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

// Main Window ----------------------------------------------------------------------------------------------------------------
//
// Description: The main window of the dash application. Instancing this will open a new window bound to the speficied CAN
//   database. This window will display specific entries of the database as they update. For a list of the used entries, see
//   below.
//
// Created: 23.08.06
// Updated: 23.08.19

// Libraries ------------------------------------------------------------------------------------------------------------------

// Includes
#include "can_database.h"
#include "strata_bar.h"
#include "can_database_table.h"

// QT Libraries
#include <QMainWindow>
#include <QTimer>

// Main window UI forward declaration
// - This namespace and class are generated at compile-time in the ui_main_window.h file
namespace Ui
{
    class MainWindow;
}

// Classes --------------------------------------------------------------------------------------------------------------------

class MainWindow : public QMainWindow
{
    Q_OBJECT // QT object boilerplate

    // Constants --------------------------------------------------------------------------------------------------------------

    public:

    #define UPDATE_INTERVAL_MS 100

    #define ID_VIEW_MENU      0
    #define ID_VIEW_DATABASE  1
    #define ID_VIEW_SPEED     2
    #define ID_VIEW_ENDURANCE 3
    #define ID_VIEW_LAP       4

    // Constructor / Destructor -----------------------------------------------------------------------------------------------

    MainWindow(Network::CanDatabase* database, QWidget* parent = nullptr);
    
    ~MainWindow();

    // Private Variables ------------------------------------------------------------------------------------------------------

    private:
    
    Ui::MainWindow* ui;

    StrataBar*        rpmBar;            // Custom RPM bar
    CanDatabaseTable* databaseTable;     // Table for CAN database

    QTimer* updateTimer;                 // Timer for the update loop

    int* barBrakePercent;
    int* barThrottlePercent;
    int* barTorquePercent;
    int* barRegenPercent;
    int* statSpeedValue;
    int* statChargeValue;
    int* motorSpeed;
    
    // Private Functions ------------------------------------------------------------------------------------------------------

    void setView(int id);

    // Slots ------------------------------------------------------------------------------------------------------------------

    private slots:

    void update();

    void handleButtonMenu()      { this->setView(ID_VIEW_MENU); }
    void handleButtonSpeed()     { this->setView(ID_VIEW_SPEED); }
    void handleButtonEndurance() { this->setView(ID_VIEW_ENDURANCE); }
    void handleButtonLap()       { this->setView(ID_VIEW_LAP); }
    void handleButtonDatabase()     { this->setView(ID_VIEW_DATABASE); }
};

#endif // MAIN_WINDOW_H
