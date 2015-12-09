//DistanceSensor.hpp
#ifndef _INCL_DISTANCESENSOR_HPP
#define _INCL_DISTANCESENSOR_HPP

#include <string>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <Log.hpp>

//----------DS::DS1----------
class DistanceSensor{
public:
        DistanceSensor(Log* Log);
        ~DistanceSensor();
        int getDistance(std::string name);
private:
        int distanceFL_ = 0;
        int distanceFR_ = 0 ;
        int distanceRL_ = 0 ;
        int distanceRR_ = 0;
        Log* logPtr;
};
//----------DS::DS2----------

#endif
