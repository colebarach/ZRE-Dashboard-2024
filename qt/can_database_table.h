#ifndef CAN_DATABASE_TABLE_H
#define CAN_DATABASE_TABLE_H

// Strata Bar -----------------------------------------------------------------------------------------------------------------
//
// Description: A table displaying the contents of a CAN database.
//
// Created: 23.08.21
// Updated: 23.08.22
//
// To do:
// - Make a decision on single label vs group. If using single, this widget doesn't really need to exist.

// Libraries ------------------------------------------------------------------------------------------------------------------

// Includes
#include "can_database.h"

// QT Libraries
#include <QWidget>
#include <QLabel>

class CanDatabaseTable : QWidget
{
    Q_OBJECT // QT object boilerplate

    // Constructor / Destructor -----------------------------------------------------------------------------------------------

    public:

    CanDatabaseTable(QWidget* parent, Network::CanDatabase* database);

    ~CanDatabaseTable();

    // Public Functions -------------------------------------------------------------------------------------------------------

    void update();

    // Protected Variables ----------------------------------------------------------------------------------------------------

    protected:

    Network::CanDatabase* database;

    // const Network::CanMessage* canMessages;
    // const Network::CanSignal*  canSignals;
    // size_t                     messageCount;
    // size_t                     signalCount;
    // QLabel**                   messageLabels;
    // QLabel**                   signalLabels;

    QLabel* testLabel;
};

#endif // CAN_DATABASE_TABLE_H