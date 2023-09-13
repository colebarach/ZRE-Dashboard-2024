#ifndef VIEW_DEBUG_H
#define VIEW_DEBUG_H

// Debug View -----------------------------------------------------------------------------------------------------------------
//
// Description: Debug menu of the GUI.
//
// Created: 23.08.23
// Updated: 23.08.26

// Libraries ------------------------------------------------------------------------------------------------------------------

// Includes
#include "view.h"

// QT Libraries
#include <QWidget>

// Classes --------------------------------------------------------------------------------------------------------------------

// View UI forward declaration
// - This namespace and class are generated at compile-time using the form file
namespace Ui
{
    class ViewDebug;
}

class ViewDebug : public View
{
    Q_OBJECT // QT object boilerplate
    
    // Constants --------------------------------------------------------------------------------------------------------------

    #define DATABASE_TEXT_SIZE 65536

    // Constructor / Destructor -----------------------------------------------------------------------------------------------

    public:

    ViewDebug(QWidget* parent_, MainWindow* mainWindow_, Network::Database* database_);

    ~ViewDebug();

    // Public Funtions --------------------------------------------------------------------------------------------------------

    void update() override;

    // Slots ------------------------------------------------------------------------------------------------------------------

    private slots:

    void handleButtonMenu();

    // Protected Variables ----------------------------------------------------------------------------------------------------

    protected:

    Ui::ViewDebug* ui;

    char databaseText[DATABASE_TEXT_SIZE];
};

#endif // VIEW_DEBUG_H