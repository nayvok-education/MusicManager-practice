#include "mainwindow.h"
#include "welcomewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    WelcomeWindow welcomeWindow;
    welcomeWindow.show();

    while (welcomeWindow.isVisible()) {
        a.processEvents();
    }

    MainWindow w;
    w.show();

    return a.exec();
}
