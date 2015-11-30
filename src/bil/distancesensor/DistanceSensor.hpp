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
#include "Log.hpp"

class DistanceSensor{
public:
        DistanceSensor(Log* log);
        ~DistanceSensor();
        int getDistance(std::string name);
private:
        Log* Log;
};

#endif
