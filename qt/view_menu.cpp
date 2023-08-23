// Header
#include "view_menu.h"

// Includes
#include "main_window.h"

// UI Includes
#include "ui_view_menu.h"

ViewMenu::ViewMenu(QWidget* parent_, MainWindow* mainWindow_, Network::Database* database_) : View(parent_, mainWindow_, database_)
{
    // Create and apply the UI
    ui = new Ui::ViewMenu;
    ui->setupUi(this);

    // Connect GUI events
    connect(ui->buttonDrive,     SIGNAL(clicked()), this, SLOT(handleButtonDrive()));
    connect(ui->buttonDebug,     SIGNAL(clicked()), this, SLOT(handleButtonDebug()));
}

ViewMenu::~ViewMenu()
{
    // Delete UI
    delete ui;
}

void ViewMenu::update() {}

void ViewMenu::handleButtonDrive()
{
    mainWindow->setView(ID_VIEW_DRIVE);
}

void ViewMenu::handleButtonDebug()
{
    mainWindow->setView(ID_VIEW_DEBUG);
}