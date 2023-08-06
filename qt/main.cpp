// Includes
#include "mainwindow.h"

// QT Libraries
#include <QApplication>

// Standard Librariess
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    std::cout << "You have entered " << argc << " arguments:" << std::endl;

    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << std::endl;
    }

    return a.exec();
}
