#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("Au2");
    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

