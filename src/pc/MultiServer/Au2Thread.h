#ifndef AU2THREAD_H
#define AU2THREAD_H

#include <QTcpSocket>
#include <QThread>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <String>
#include <QMutex>

class Au2Thread : public QThread
{
    Q_OBJECT
public:
    explicit Au2Thread(int ID, QObject *parent = 0,std::string type_ = 0, QMutex *mutex_ = 0);
    void run();
signals:

public slots:
    int readyRead();
    void disconnect();

private:
        QTcpSocket *socket;
        QMutex *mutex;
        std::string type;
        int socketDesciptor;
};

#endif // AU2THREAD_H
