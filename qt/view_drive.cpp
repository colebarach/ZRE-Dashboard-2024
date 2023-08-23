// Header
#include "view_drive.h"

// Includes
#include "main_window.h"

// UI Includes
#include "ui_view_drive.h"

ViewDrive::ViewDrive(QWidget* parent_, MainWindow* mainWindow_, Network::Database* database_) : View(parent_, mainWindow_, database_)
{
    // Create and apply the UI
    ui = new Ui::ViewDrive;
    ui->setupUi(this);

    // Create custom widgets
    rpmBar = new StrataBar(ui->strataBarRpm);

    // Get database references
    motorSpeed         = database->reference<int>(MOTOR_SPEED);
    motorMph           = database->reference<int>(MOTOR_MPH);
    stateOfCharge      = database->reference<int>(STATE_OF_CHARGE);
    tempAccumulatorMax = database->reference<int>(TEMP_ACC_MAX);
    tempInverterMax    = database->reference<int>(TEMP_INV_MAX);
    tempMotorMax       = database->reference<int>(TEMP_MOTOR_MAX);
    throttlePercent    = database->reference<int>(THROTTLE_PERCENT);
    brakePercent       = database->reference<int>(BRAKE_PERCENT);
    torquePercent      = database->reference<int>(TORQUE_PERCENT);
    regenPercent       = database->reference<int>(REGEN_PERCENT);

    // Connect GUI events
    connect(ui->buttonSpeed,     SIGNAL(clicked()), this, SLOT(handleButtonSpeed()));
    connect(ui->buttonEndurance, SIGNAL(clicked()), this, SLOT(handleButtonEndurance()));
    connect(ui->buttonMenu,      SIGNAL(clicked()), this, SLOT(handleButtonMenu()));
}

ViewDrive::~ViewDrive()
{
    // Delete UI
    delete ui;

    // Delete custom widgets
    delete rpmBar;
}

void ViewDrive::update()
{
    rpmBar->setValue(*motorSpeed);
    ui->statSpeed->setText(QString::number(*motorMph));
    ui->statCharge->setText(QString::number(*stateOfCharge));
    ui->statCharge2->setText(QString::number(*stateOfCharge));
    ui->statTempAccumulatorMax->setText(QString::number(*tempAccumulatorMax));
    ui->statTempInverterMax->setText(QString::number(*tempInverterMax));
    ui->statTempMotorMax->setText(QString::number(*tempMotorMax));

    ui->barThrottle->setValue(*throttlePercent);
    ui->barBrake->setValue(*brakePercent);
    ui->barTorque->setValue(*torquePercent);
    ui->barRegen->setValue(*regenPercent);
}

void ViewDrive::handleButtonSpeed()
{
    ui->frameSubviews->setCurrentWidget(ui->subviewSpeed);
}

void ViewDrive::handleButtonEndurance()
{
    ui->frameSubviews->setCurrentWidget(ui->subviewEndurance);
}

void ViewDrive::handleButtonMenu()
{
    mainWindow->setView(ID_VIEW_MENU);
}