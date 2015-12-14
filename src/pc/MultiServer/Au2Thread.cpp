#include "Au2Thread.h"
#include "windows.h"
#include <stdlib.h>
#include <unistd.h>
#include <QProcess>

Au2Thread::Au2Thread(int ID, QObject *parent, std::string type_, QMutex *mutex_) : QThread(parent), type(type_), mutex(mutex_)
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

//----------readyRead0----------
int Au2Thread::readyRead()
{
    COORD coord;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (type == "Data")
    {
    mutex->lock();
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
        socket->write("dwnnow",6);
        coord.X = 0;
        coord.Y = 0;
        SetConsoleCursorPosition(hConsole, coord);
        QProcess::execute("CLS");
        std::cout << "System has been restarted"<<std::endl;
        std::cout << "Ready for new connection.."<< std::endl;
    }
    else
    {
        coord.X = 0;
        coord.Y = 5;
        SetConsoleCursorPosition(hConsole, coord);
        std::cout << "Recived data.. \r\n Makshastighed: "<< (int)Data[0]<<" Hastighed: "<< (int)Data[1]<<" Afstand: "
                     << (int)Data[2]<<" Acceleration: "<< (int)Data[3]<<" AKSstatus: "<< (int)Data[4]<<" Styretoj: "
                     << (int)Data[5]<<"    \r\n";
        char wData[6];
        wData[0]=Data[0];
        wData[1]=rand() % 100 + 1;
        wData[2]=rand() % 100 + 1;
        wData[3]=rand() % 100 + 1;
        wData[4]=Data[4];
        wData[5]=Data[5];
        std::cout << "Sending new data..    "<< "\r\n";
        socket->write(wData,6);
    }
    mutex->unlock();
    }
   else if (type == "Controller")
    {
        mutex->lock();
        char Data[4];
        socket->read(Data,4);
        /*
         data[0] = turn
         data[1] = speed
         data[2] = brake
        */
        coord.X = 0;
        coord.Y = 10;
        SetConsoleCursorPosition(hConsole, coord);
        std::cout << "Recived controller data.. \r\n Drej: "<< (int)Data[2]<<" Tilbage: "<< (int)(Data[1])<<" Frem: "<< (int)(Data[0])<<" Stop: "
                  << (int)Data[3]<<"            \r\n";
        mutex->unlock();
    }
    return 0;
}
//----------readyRead1----------

void Au2Thread::disconnect()
{
    socket->deleteLater();
    exit(0);
}


