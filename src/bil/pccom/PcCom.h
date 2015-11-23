/* The functionality of the PcCom class is to communicate
 * between the PC and the Raspberry Pi through a TCP-connection */

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
#include "Data.h"
#include "Settings.h"


#ifndef PCCOM_H
#define PCCOM_H

class pcCom
{
public:
    pcCom(dataTest* dataClassPtr, settingsTest* settingsClassPtr);
    ~pcCom();
    int controllerStream();
    int dataStream();
    void error(const char *msg );

private:
    char data_[6];
    char controller_[4];
    int portnoData_;
    int portnoController_;
    dataTest* dataClassPtr_;
    settingsTest* settingsClassPtr_;
};

#endif // PCCOM_H
