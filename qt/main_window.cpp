// Header
#include "main_window.h"

// Includes
#include "config.h"

// QT Includes
// - These files are generated in the compilation of the project, they implement the UI created in the form files
#include "ui_main_window.h"

// QT Libraries
#include <QWidget>
#include <QString>

// C++ Standard Libraries
#include <string>
#include <exception>
#include <stdexcept>

MainWindow::MainWindow(Network::CanDatabase* database, QWidget* parent) : QMainWindow(parent)
{
    // Create and apply the UI
    ui = new Ui::MainWindow;
    ui->setupUi(this);
    
    // Set the build title
    // - The UI overrides the title, so this must be done after applying
    setWindowTitle(QString::fromStdString(std::string(BUILD_TITLE) + " - " + __DATE__ + " - QT Frontend - Rev." + BUILD_REVISION));

    // Create custom widgets
    rpmBar = new StrataBar(ui->strataBarRpm);
    databaseTable = new CanDatabaseTable(ui->canDatabaseTable, database);

    // Create the update timer
    updateTimer = new QTimer(this);

    // Connect GUI events
    connect(updateTimer,              SIGNAL(timeout()), this, SLOT(update()));

    connect(ui->menuButtonSpeed,      SIGNAL(clicked()), this, SLOT(handleButtonSpeed()));
    connect(ui->menuButtonEndurance,  SIGNAL(clicked()), this, SLOT(handleButtonEndurance()));
    connect(ui->menuButtonLap,        SIGNAL(clicked()), this, SLOT(handleButtonLap()));
    connect(ui->menuButtonDatabase,   SIGNAL(clicked()), this, SLOT(handleButtonDatabase()));

    connect(ui->driveButtonMenu,      SIGNAL(clicked()), this, SLOT(handleButtonMenu()));
    connect(ui->driveButtonSpeed,     SIGNAL(clicked()), this, SLOT(handleButtonSpeed()));
    connect(ui->driveButtonEndurance, SIGNAL(clicked()), this, SLOT(handleButtonEndurance()));
    connect(ui->driveButtonLap,       SIGNAL(clicked()), this, SLOT(handleButtonLap()));

    connect(ui->databaseButtonMenu,   SIGNAL(clicked()), this, SLOT(handleButtonMenu()));

    // Get database references
    // - TODO: These names should be macros in the header
    barThrottlePercent = database->reference<int>("APPS_1_Percent");
    barBrakePercent    = database->reference<int>("Brake_1_Percent");
    barTorquePercent   = database->reference<int>("Torque_Config_Limit");
    barRegenPercent    = database->reference<int>("Torque_Config_Limit_Regen");
    statSpeedValue     = database->reference<int>("Motor_Speed");
    statChargeValue    = database->reference<int>("State_of_Charge");
    motorSpeed         = database->reference<int>("Motor_Speed");

    // Start the update timer
    updateTimer->start(UPDATE_INTERVAL_MS);
}

MainWindow::~MainWindow()
{
    // Delete UI
    delete ui;

    // Delete custom widgets
    delete rpmBar;
    delete databaseTable;

    // Delete objects
    delete updateTimer;
}

void MainWindow::update()
{
    // Update widget values
    ui->barBrake->setValue(*barBrakePercent);
    ui->barThrottle->setValue(*barThrottlePercent);
    ui->barTorque->setValue(*barTorquePercent);
    ui->barRegen->setValue(*barRegenPercent);

    ui->statSpeed->setText(QString::number(*statSpeedValue));
    ui->statCharge->setText(QString::number(*statChargeValue));

    rpmBar->setValue(*motorSpeed);

    databaseTable->update();

    // Restart the timer
    updateTimer->start(UPDATE_INTERVAL_MS);
}

void MainWindow::setView(int id)
{
    switch(id)
    {
        case ID_VIEW_MENU:
            ui->frameViews->setCurrentWidget(ui->viewMenu);
            break;
        case ID_VIEW_SPEED:
            ui->frameViews->setCurrentWidget(ui->viewDrive);
            ui->frameSubViews->setCurrentWidget(ui->subViewSpeed);
            break;
        case ID_VIEW_ENDURANCE:
            ui->frameViews->setCurrentWidget(ui->viewDrive);
            ui->frameSubViews->setCurrentWidget(ui->subViewEndurance);
            break;
        case ID_VIEW_LAP:
            break;
        case ID_VIEW_DATABASE:
            ui->frameViews->setCurrentWidget(ui->viewDatabase);
            break;
        default:
            throw std::runtime_error("Failed to set view: Unknown view ID " + std::to_string(id));
    }
}