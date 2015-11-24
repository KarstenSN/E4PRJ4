#include "Au2Server.h"

Au2Server::Au2Server(int port_, std::string type_, QMutex *mutex_): port(port_), type(type_), mutex(mutex_)
{

}

void Au2Server::StartServer()
{
    if(!this->listen(QHostAddress::Any,port))
    {
        std::cout <<"Could not start" <<type<<" server\r\n";
    }
    else
    {
        std::cout <<type<<" server startet\r\n";
    }

}

void Au2Server::incomingConnection(int socketDescriptor)
{
   // std::cout << "New connection: " << socketDescriptor << std::endl;
    Au2Thread * thread = new Au2Thread(socketDescriptor,this,type,mutex);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

