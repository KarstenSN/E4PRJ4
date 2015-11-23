#include <QCoreApplication>
#include "Au2Server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Au2Server Server;
    Server.StartServer();

    return a.exec();
}
