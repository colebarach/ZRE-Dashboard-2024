// Header
#include "stat_cell.h"

// QT Libraries
#include <QLayout>
#include <QGridLayout>

StatCell::StatCell(QWidget* parent) : QFrame(parent)
{
    framePalette = new QPalette(palette());
    framePalette->setColor(QPalette::ColorRole::Window, Qt::red);
    setAutoFillBackground(true);
    setPalette(*framePalette);

    // Create voltage label
    voltageStat = new QLabel(this);
    
    // Apply layout
    QGridLayout* layout_ = new QGridLayout();
    layout_->addWidget(voltageStat);
    setLayout(layout_);

    voltageStat->setText("1.\n00");
}

StatCell::~StatCell()
{
    delete voltageStat;
}