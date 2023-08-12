// Header
#include "main_window.h"

// QT Includes
// - This file is generated in the compilation of the project, it implements the UI created in the main_window.ui file
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

    // Create the update timer
    updateTimer = new QTimer(this);

    // Connect GUI events
    connect(updateTimer,              SIGNAL(timeout()), this, SLOT(update()));
    
    connect(ui->menuButtonSpeed,      SIGNAL(clicked()), this, SLOT(handleButtonSpeed()));
    connect(ui->menuButtonEndurance,  SIGNAL(clicked()), this, SLOT(handleButtonEndurance()));
    connect(ui->menuButtonLap,        SIGNAL(clicked()), this, SLOT(handleButtonLap()));
    connect(ui->menuButtonDebug,      SIGNAL(clicked()), this, SLOT(handleButtonDebug()));

    connect(ui->driveButtonMenu,      SIGNAL(clicked()), this, SLOT(handleButtonMenu()));
    connect(ui->driveButtonSpeed,     SIGNAL(clicked()), this, SLOT(handleButtonSpeed()));
    connect(ui->driveButtonEndurance, SIGNAL(clicked()), this, SLOT(handleButtonEndurance()));
    connect(ui->driveButtonLap,       SIGNAL(clicked()), this, SLOT(handleButtonLap()));

    connect(ui->debugButtonMenu,      SIGNAL(clicked()), this, SLOT(handleButtonMenu()));

    // Get database references
    // - TODO: These names should be macros in the header
    barThrottlePercent = database->reference<int>("APPS_1_Percent");
    barBrakePercent    = database->reference<int>("Brake_1_Percent");
    barTorquePercent   = database->reference<int>("Torque_Config_Limit");
    barRegenPercent    = database->reference<int>("Torque_Config_Limit_Regen");
    statSpeedValue     = database->reference<int>("Motor_Speed");
    statChargeValue    = database->reference<int>("State_of_Charge");

    // Start the update timer
    updateTimer->start(UPDATE_INTERVAL_MS);
}

MainWindow::~MainWindow()
{
    // Delete widgets
    delete ui;
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
        case ID_VIEW_DEBUG:
            ui->frameViews->setCurrentWidget(ui->viewDebug);
            break;
        default:
            throw std::runtime_error("Failed to set view: Unknown view ID " + std::to_string(id));
    }
}