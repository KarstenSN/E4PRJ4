#ifndef AU2SERVER_H
#define AU2SERVER_H

#include <QTcpServer>
#include "Au2Thread.h"


class Au2Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Au2Server(QObject *parent = 0);

public slots:
    void StartServer();

protected:
    void incomingConnection(int socketDescriptor);

};

#endif // AU2SERVER_H
