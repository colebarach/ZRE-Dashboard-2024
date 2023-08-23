#ifndef VIEW_H
#define VIEW_H

// View -----------------------------------------------------------------------------------------------------------------------
//
// Description: A view of the GUI. Correlates to a single UI file of the project.
//
// Created: 23.08.22
// Updated: 23.08.23

// Libraries ------------------------------------------------------------------------------------------------------------------

// Includes
#include "database.h"

// QT Libraries
#include <QWidget>

// Classes --------------------------------------------------------------------------------------------------------------------

// Main window forward declaration
// - Todo: this isn't the best solution
class MainWindow;

class View : public QWidget
{
    Q_OBJECT // QT object boilerplate

    // Constructor / Destructor -----------------------------------------------------------------------------------------------

    public:

    View(QWidget* parent_, MainWindow* mainWindow_, Network::Database* database_) : QWidget(parent_)
    {
        mainWindow = mainWindow_;
        database   = database_;
    }

    // Virtual Functions ------------------------------------------------------------------------------------------------------

    virtual void update() = 0;

    // Protected Variables ----------------------------------------------------------------------------------------------------

    protected:

    MainWindow*        mainWindow;
    Network::Database* database;
};

#endif // VIEW_H