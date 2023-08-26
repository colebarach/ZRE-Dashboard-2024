// Header
#include "view_debug.h"

// Includes
#include "main_window.h"

// UI Includes
#include "ui_view_debug.h"

ViewDebug::ViewDebug(QWidget* parent_, MainWindow* mainWindow_, Network::Database* database_) : View(parent_, mainWindow_, database_)
{
    // Create and apply the UI
    ui = new Ui::ViewDebug;
    ui->setupUi(this);

    // Connect GUI events
    connect(ui->buttonMenu, SIGNAL(clicked()), this, SLOT(handleButtonMenu()));
}

ViewDebug::~ViewDebug()
{
    // Delete UI
    delete ui;
}

void ViewDebug::update()
{
    database->print(databaseText, DATABASE_TEXT_SIZE);
    ui->labelCanDatabase->setText(QString(databaseText));
}

void ViewDebug::handleButtonMenu()
{
    mainWindow->setView(ID_VIEW_MENU);
}