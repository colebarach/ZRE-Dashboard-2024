// Header
#include "view_bms.h"

// Includes
#include "main_window.h"

// UI Includes
#include "ui_view_bms.h"

// QT Libraries
#include <QLayout>
#include <QGridLayout>

ViewBms::ViewBms(QWidget* parent_, MainWindow* mainWindow_, Network::Database* database_) : View(parent_, mainWindow_, database_)
{
    // Create and apply the UI
    ui = new Ui::ViewBms;
    ui->setupUi(this);

    QGridLayout* layout = new QGridLayout(ui->frameCenter);

    layout->setSpacing(3);

    cells = new StatCell**[SEGMENT_COUNT];
    for(size_t segmentIndex = 0; segmentIndex < SEGMENT_COUNT; ++segmentIndex)
    {
        cells[segmentIndex] = new StatCell*[CELLS_PER_SEGMENT];

        for(size_t cellIndex = 0; cellIndex < CELLS_PER_SEGMENT; ++cellIndex)
        {
            cells[segmentIndex][cellIndex] = new StatCell(ui->frameCenter);

            layout->addWidget(cells[segmentIndex][cellIndex], segmentIndex, cellIndex);
        }
    }

    ui->frameCenter->setLayout(layout);

    // Connect GUI events
    connect(ui->buttonMenu, SIGNAL(clicked()), this, SLOT(handleButtonMenu()));
}

ViewBms::~ViewBms()
{
    // Delete UI
    delete ui;

    // TODO: Delete everything
}

void ViewBms::update() {}

void ViewBms::handleButtonMenu()
{
    mainWindow->setView(ID_VIEW_MENU);
}