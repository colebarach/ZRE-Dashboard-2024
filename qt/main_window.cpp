// Header
#include "main_window.h"

// QT Includes
// - This file is generated in the compilation of the project, it implements the UI created in the main_window.ui file
#include "ui_main_window.h"

MainWindow::MainWindow(Network::CanDatabase* database, QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // Apply the UI
    ui->setupUi(this);

    updateTimer = new QTimer(this);
    updateTimer->start(UPDATE_INTERVAL_MS);

    connect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));

    barThrottlePercent = database->reference<int>("APPS_1_Percent");
    barBrakePercent    = database->reference<int>("Brake_1_Percent");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update()
{
    ui->barBrake->setValue(*barBrakePercent);
    ui->barThrottle->setValue(*barThrottlePercent);

    // Continue loop
    updateTimer->start(UPDATE_INTERVAL_MS);
}

