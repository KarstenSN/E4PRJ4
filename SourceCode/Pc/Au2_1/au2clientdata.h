#ifndef AU2CLIENTDATA_H
#define AU2CLIENTDATA_H

#include <QThread>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QMutex>


class Au2clientData : public QThread
{
    Q_OBJECT

public:
    Au2clientData();
    int connect(QTcpSocket *_socket,QString *_IP);
    int getData(int *_makshastighed, int *_hastighed, int *_afstand, int *_acceleration,
                int *_AKSStatus, int *_styretoj);
    void connected();
    void disconnected();
    void stop();
    ~Au2clientData();

signals:
    void dataUpdated();
    void connectionLost();

protected:
    void run();

private:
    QMutex mutex;
    QTcpSocket *socket;
    char data[6];
    volatile bool stopped;
    volatile bool _connected;
};

#endif // AU2CLIENTDATA_H

