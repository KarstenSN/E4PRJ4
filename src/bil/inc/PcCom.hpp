/* The functionality of the PcCom class is to communicate
 * between the PC and the Raspberry Pi through a TCP-connection.
 * It is able to open two sperate TCP connections, one for
 * UserInput and one for Data */

#ifndef PCCOM_HPP
#define PCCOM_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <thread>
#include <Data.hpp>
#include <Settings.hpp>
#include <utilities.hpp>
#include <Log.hpp>

class PcCom
{
public:
    PcCom(Data* dataClassPtr, Settings* settingsClassPtr, Log* logClassPtr);
    ~PcCom();

private:
    void controllerStream();
    void dataStream();
    void error(std::string msg);
    
    char data_[6];
    char controller_[4];
    int portnoData_;
    int portnoController_;
    Data* dataClassPtr_;
    Settings* settingsClassPtr_;
    UserInput UserInput_;
    Log* logClassPtr_;
};

#endif // PCCOM_HPP
