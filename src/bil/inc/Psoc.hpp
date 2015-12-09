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

//----------psoc::header1----------
class Psoc{
public:
		Psoc(Log* Log);
		~Psoc();
		int getDistance(std::string name);
		int getVelocity();
private:
		void psocRead();
		int update();
		int distanceFL_ = 0;
		int distanceFR_ = 0;
		int distanceRL_ = 0;
		int distanceRR_ = 0 ;
		int Tacho_ = 0;
		Log* logPtr_;
		std::mutex psocMut;
		std::thread psocThread_;
};
//----------psoc::header2----------

#endif