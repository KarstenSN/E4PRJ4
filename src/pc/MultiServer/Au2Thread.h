#ifndef AU2THREAD_H
#define AU2THREAD_H

#include <QTcpSocket>
#include <QThread>
#include <iostream>
#include <stdlib.h>
#include <time.h>

class Au2Thread : public QThread
{
    Q_OBJECT
public:
    explicit Au2Thread(int ID, QObject *parent = 0);
    void run();
signals:

public slots:
    int readyRead();
    void disconnect();

private:
        QTcpSocket *socket;
        int socketDesciptor;
};

#endif // AU2THREAD_H
