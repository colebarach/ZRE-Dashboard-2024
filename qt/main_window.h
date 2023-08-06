#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

// Main Window ----------------------------------------------------------------------------------------------------------------
//
// Description: The window of the dash application.
//
// Created: 23.08.06
// Updated: 23.08.06

// Libraries ------------------------------------------------------------------------------------------------------------------

// Includes
#include "can_database.h"

// QT Libraries
#include <QMainWindow>
#include <QTimer>
#include <QProgressBar>

// Main window UI forward declaration
// - This namespace and class are generated at compile-time in the ui_main_window.h file
namespace Ui
{
    class MainWindow;
}

// Classes --------------------------------------------------------------------------------------------------------------------

class MainWindow : public QMainWindow
{
    // QT object boilerplate
    Q_OBJECT

    public:

    // Constants --------------------------------------------------------------------------------------------------------------

    #define UPDATE_INTERVAL_MS 100

    // Constructor / Destructor -----------------------------------------------------------------------------------------------

    MainWindow(Network::CanDatabase* database, QWidget* parent = nullptr);
    
    ~MainWindow();

    // Private Variables ------------------------------------------------------------------------------------------------------

    private:
    
    Ui::MainWindow *ui;

    QTimer* updateTimer;

    int* barBrakePercent;
    int* barThrottlePercent;
    
    // Slots ------------------------------------------------------------------------------------------------------------------

    public slots:

    void update();
};

#endif // MAIN_WINDOW_H
