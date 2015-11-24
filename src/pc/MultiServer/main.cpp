#include <QCoreApplication>
#include "Au2Server.h"
#include <QMutex>

int main(int argc, char *argv[])
{
    QMutex mutex;

    QCoreApplication a(argc, argv);

    Au2Server dataServer(1234, "Data",&mutex);
    dataServer.StartServer();

    Au2Server controllerServer(1235, "Controller",&mutex);
    controllerServer.StartServer();

    return a.exec();
}
