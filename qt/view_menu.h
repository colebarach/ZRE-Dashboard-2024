#ifndef VIEW_MENU_H
#define VIEW_MENU_H

// Menu View ------------------------------------------------------------------------------------------------------------------
//
// Description: The main menu of the GUI.
//
// Created: 23.08.23
// Updated: 23.08.23
//

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
    class ViewMenu;
}

class ViewMenu : public View
{
    Q_OBJECT // QT object boilerplate
    
    // Constructor / Destructor -----------------------------------------------------------------------------------------------

    public:

    ViewMenu(QWidget* parent_, MainWindow* mainWindow_, Network::Database* database_);

    ~ViewMenu();

    // Public Funtions --------------------------------------------------------------------------------------------------------

    void update() override;

    // Slots ------------------------------------------------------------------------------------------------------------------

    private slots:

    void handleButtonDrive();

    void handleButtonDebug();

    // Protected Variables ----------------------------------------------------------------------------------------------------

    protected:

    Ui::ViewMenu* ui;
};

#endif // VIEW_MENU_H