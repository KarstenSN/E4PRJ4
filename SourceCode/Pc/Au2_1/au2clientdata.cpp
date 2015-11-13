#include "au2clientdata.h"

Au2clientData::Au2clientData()
{

}

void Au2clientData::run()
{
    while((!stopped)&&(_connected))
    {
        mutex.lock();
        socket->write(data,6);
        socket->waitForBytesWritten();
        socket->waitForReadyRead();
        socket->read(data,6);
        mutex.unlock();
        dataUpdated();
        QThread::msleep(100);
    }
}

void Au2clientData::stop()
{
    stopped = true;
}

int Au2clientData::connect(QTcpSocket *_socket,QString *_IP)
{  
    socket =_socket;
    _socket->connectToHost(*_IP,1234);
    if(!_socket->waitForConnected(1000))
        return -1;

    return 0;
}

int Au2clientData::getData(int *_makshastighed, int *_hastighed, int *_afstand, int *_acceleration,
                           int *_AKSStatus, int *_styretoj)
{
    mutex.lock();
    data[0]=(char)*_makshastighed;
    *_hastighed=(int)data[1];
    *_afstand=(int)data[2];
    *_acceleration=(int)data[3];
    data[4]=(char)*_AKSStatus;
    data[5]=(char)*_styretoj;
    mutex.unlock();
    return 0;
}

void Au2clientData::connected()
{
    _connected = true;
}

void Au2clientData::disconnected()
{
    _connected = false;
    socket->deleteLater();
}


Au2clientData::~Au2clientData()
{
    delete socket;
}
