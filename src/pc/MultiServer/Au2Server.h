#ifndef AU2SERVER_H
#define AU2SERVER_H

#include <QTcpServer>
#include "Au2Thread.h"
#include <String>
#include <QMutex>

class Au2Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Au2Server(int port_, std::string type_, QMutex *mutex_);

public slots:
    void StartServer();

protected:
    void incomingConnection(int socketDescriptor);

private:
    int port;
    std::string type;
    QMutex *mutex;

};

#endif // AU2SERVER_H
