#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

// Main Window ----------------------------------------------------------------------------------------------------------------
//
// Description: The main window of the dash application. Instancing this will open a new window bound to the speficied database
//   This window will display specific entries of the database as they update. For a list of the used entries, see below.
//
// Created: 23.08.06
// Updated: 23.08.23
//
// To do:
// - Only update visible views

// Libraries ------------------------------------------------------------------------------------------------------------------

// Includes
#include "database.h"
#include "view.h"

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

    #define ID_VIEW_MENU     0
    #define ID_VIEW_DEBUG    1
    #define ID_VIEW_DRIVE    2
    #define ID_VIEW_SETTINGS 3

    // Constructor / Destructor -----------------------------------------------------------------------------------------------

    MainWindow(Network::Database* database, QWidget* parent = nullptr);
    
    ~MainWindow();

    // Private Functions ------------------------------------------------------------------------------------------------------

    void setView(int viewId);

    // Slots ------------------------------------------------------------------------------------------------------------------

    private slots:

    void update();

    // Private Variables ------------------------------------------------------------------------------------------------------

    private:
    
    Ui::MainWindow*    ui;               // UI of the window, sourced from main_window.ui
    View*              viewMenu;         // Menu view, instance from view_menu.h
    View*              viewDrive;        // Driving view, instance from view_drive.h
    View*              viewDebug;        // Debug view, instance from view_debug.h

    Network::Database* database;         // Database to be visualized

    QTimer* updateTimer;                 // Timer for the update loop
};

#endif // MAIN_WINDOW_H
