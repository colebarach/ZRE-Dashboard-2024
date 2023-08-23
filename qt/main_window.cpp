// Header
#include "main_window.h"

// Includes
#include "config.h"
#include "view_drive.h"

// UI Includes
#include "ui_main_window.h"

// QT Libraries
#include <QWidget>
#include <QString>

// C++ Standard Libraries
#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

MainWindow::MainWindow(Network::Database* database, QWidget* parent) : QMainWindow(parent)
{
    // Create and apply the UI
    ui = new Ui::MainWindow;
    ui->setupUi(this);
    
    // Set the build title
    // - The UI overrides the title, so this must be done after applying
    setWindowTitle(QString::fromStdString(std::string(BUILD_TITLE) + " - " + __DATE__ + " - QT Frontend - Rev." + BUILD_REVISION));

    // Create views
    viewDrive = new ViewDrive(ui->frameViews, this, database);
    ui->frameViews->addWidget(viewDrive);

    // Create the update timer
    updateTimer = new QTimer(this);

    // Connect GUI events
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));

    // Start the update timer
    updateTimer->start(UPDATE_INTERVAL_MS);
}

MainWindow::~MainWindow()
{
    // Delete UI
    delete ui;

    // Delete objects
    delete updateTimer;
}

void MainWindow::setView(int viewId)
{
    switch(viewId)
    {
        case ID_VIEW_MENU:
            // ui->frameViews->setCurrentWidget();
            std::cout << "Menu..." << std::endl;
            break;
        case ID_VIEW_DEBUG:
            // ui->frameViews->setCurrentWidget();
            break;
        case ID_VIEW_DRIVE:
            ui->frameViews->setCurrentWidget(viewDrive);
            break;
        case ID_VIEW_SETTINGS:
            // ui->frameViews->setCurrentWidget();
            break;
        default:
            throw std::runtime_error("Failed to set view: Unknown view ID " + std::to_string(viewId));
    }
}

void MainWindow::update()
{
    viewDrive->update();

    // Restart the timer
    updateTimer->start(UPDATE_INTERVAL_MS);
}