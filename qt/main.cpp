// Includes
// - The CAN database must be included first, as including the main window will define a macro affecting the compliation of the
//   code in the CAN database header.
#include "can_database.h"
#include "main_window.h"
#include "log.h"

// QT Libraries
#include <QApplication>

// C++ Libraries
#include <exception>

int main(int argc, char *argv[])
{
    try
    {
        QApplication application(argc, argv);

        Network::CanDatabase database(argv[2], argv[1]);

        MainWindow window(&database);
        window.show();

        return application.exec();
    }
    catch(const std::exception& exception)
    {
        LOG_FATAL("%s\n", exception.what());
    }
}
