//Psoc.hpp
#ifndef _INCL_Psoc_HPP
#define _INCL_Psoc_HPP

#include <string>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <iostream>
#include <stdlib.h>
#include <Log.hpp>

class Psoc{
public:
        Psoc(Log* Log);
        ~Psoc();
        int getDistance(std::string name);
        int getVelocity();
private:
        int update();
        static void psocRead();
        int distanceFL_;
        int distanceFR_;
        int distanceRL_;
        int distanceRR_;
        int Tacho_;
        Log* logPtr_;
        std::mutex mtx;
        
        std::thread psocThread_;
};

#endif