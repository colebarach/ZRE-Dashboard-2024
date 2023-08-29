// Header
#include "main_window.h"

// Includes
#include "config.h"
#include "view_menu.h"
#include "view_drive.h"
#include "view_debug.h"

// UI Includes
#include "ui_main_window.h"

// QT Libraries
#include <QWidget>
#include <QString>

// C++ Standard Libraries
#include <string>
#include <exception>
#include <stdexcept>

MainWindow::MainWindow(Network::Database* database, QWidget* parent) : QMainWindow(parent)
{
    // Create and apply the UI
    ui = new Ui::MainWindow;
    ui->setupUi(this);
    
    // Set the build title
    // - The UI overrides the title, so this must be done after applying
    setWindowTitle(QString::fromStdString(std::string(BUILD_TITLE) + " - " + __DATE__ + " - QT Frontend - Rev." + BUILD_REVISION));

    // Create views
    viewMenu  = new ViewMenu(ui->frameViews, this, database);
    viewDrive = new ViewDrive(ui->frameViews, this, database);
    viewDebug = new ViewDebug(ui->frameViews, this, database);

    ui->frameViews->addWidget(viewMenu);
    ui->frameViews->addWidget(viewDrive);
    ui->frameViews->addWidget(viewDebug);

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
            ui->frameViews->setCurrentWidget(viewMenu);
            break;
        case ID_VIEW_DEBUG:
            ui->frameViews->setCurrentWidget(viewDebug);
            break;
        case ID_VIEW_DRIVE:
            ui->frameViews->setCurrentWidget(viewDrive);
            break;
        case ID_VIEW_SETTINGS:
            break;
        default:
            throw std::runtime_error("Failed to set view: Unknown view ID " + std::to_string(viewId));
    }
}

void MainWindow::update()
{
    viewMenu->update();
    viewDrive->update();
    viewDebug->update();

    // Restart the timer
    updateTimer->start(UPDATE_INTERVAL_MS);
}