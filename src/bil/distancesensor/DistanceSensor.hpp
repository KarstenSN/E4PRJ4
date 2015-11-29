//DistanceSensor.hpp
#ifndef _INCL_DISTANCESENSOR_HPP
#define _INCL_DISTANCESENSOR_HPP

#include <string>

class DistanceSensor{
public:
        DistanceSensor();
        ~DistanceSensor();
        char getDistance(std::string name);
private:
        int addrFL;
        int addrFR;
        int addrRL;
        int addrRR;
        int distanceFL;
        int distanceFR;
        int distanceRL;
        int distanceRR;
        int fd;
        void returnDistance();
};

#endif
