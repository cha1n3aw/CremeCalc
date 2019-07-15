#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow calc_window;
    calc_window.show();
    return a.exec();
}


