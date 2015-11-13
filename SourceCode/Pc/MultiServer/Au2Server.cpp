#include "Au2Server.h"

Au2Server::Au2Server(QObject *parent): QTcpServer(parent)
{
}

void Au2Server::StartServer()
{
    if(!this->listen(QHostAddress::Any,1234))
    {
        std::cout <<"Could not start server\r\n";
    }
    else
    {
        std::cout <<"Server startet\r\n";
    }

}

void Au2Server::incomingConnection(int socketDescriptor)
{
    std::cout << "New connection: " << socketDescriptor << std::endl;
    Au2Thread * thread = new Au2Thread(socketDescriptor,this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

