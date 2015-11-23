#include "Au2Thread.h"

Au2Thread::Au2Thread(int ID, QObject *parent) : QThread(parent)
{
    this->socketDesciptor = ID;
    /* initialize random seed: */
    srand(time(NULL));
}

void Au2Thread::run()
{
    socket = new QTcpSocket;

    if(!socket->setSocketDescriptor(this->socketDesciptor))
        std::cout <<"Error in setSocketDescriptor\r\n";

    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()),Qt::DirectConnection);
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnect()),Qt::DirectConnection);

   exec();
}

int Au2Thread::readyRead()
{
    char Data[6];

    /*
     data[0] = makshastighed
     data[1] = hastighed
     data[2] = afstand
     data[3] = acceleration
     data[4] = AKSstatus
     data[5] = styretoj
    */

    socket->read(Data,6);
    if(Data[0]=='d' && Data[1]=='w' && Data[2]=='n' && Data[3]=='n' && Data[4]=='o' && Data[5]=='w')
    {
        std::cout << "Recived data.. \r\n Shutting down"<<std::endl;
        std::cout << "Sending dwn reply.."<< std::endl;
        socket->write("dwnnow",6);
    }
    else
    {
        std::cout << "Recived data.. \r\n Makshastighed: "<< (int)Data[0]<<" Hastighed: "<< (int)Data[1]<<" Afstand: "
                     << (int)Data[2]<<" Acceleration: "<< (int)Data[3]<<" AKSstatus: "<< (int)Data[4]<<" Styretoj: "
                     << (int)Data[5]<<std::endl;
        char wData[6];
        wData[0]=Data[0];
        wData[1]=rand() % 10 + 1;
        wData[2]=rand() % 10 + 1;
        wData[3]=rand() % 10 + 1;
        wData[4]=Data[4];
        wData[5]=Data[5];
        std::cout << "Sending new data.."<< std::endl;
        socket->write(wData,6);
    }
    return 0;
}

void Au2Thread::disconnect()
{
    socket->deleteLater();
    exit(0);
}


