// Includes
#include "main_window.h"
#include "can_database.h"

// QT Libraries
#include <QApplication>

// Standard Librariess
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    
    Network::CanDatabase database(argv[2], argv[1]);

    MainWindow window(&database);
    window.show();

    return application.exec();
}
