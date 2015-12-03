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

class DistanceSensor{
public:
        DistanceSensor(Log* Log);
        ~DistanceSensor();
        int getDistance(std::string name);
private:
        Log* Log_;
        int FL_;
        int FR_;
        int RL_;
        int RR_;
        int tacho_;
};

#endif
