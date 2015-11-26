//DistanceSensor.cpp

#include "DistanceSensor.hpp"
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int addrFL = 0b11100000;	// 0x70 dec 112
int addrFR = 0b11100001;	// 0x71 dec 113
int addrRL = 0b11100011;	// 0x73 dec 115
int addrRR = 0b11100110;	// 0x76 dec 118
int distanceFL = 0;	
int distanceFR = 0;
int distanceRL = 0;
int distanceRR = 0;

int fd;
int adapter_nr = 1; /* dynamisk allokering??? */
char filename[20];


DistanceSensor::DistanceSensor()
{
	// open i2c device;
	snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);	
	std::cout << "opening: "<< filename << std::endl;

	if (int fd = open(filename, O_RDWR) < 0);
	{
		std::cout << "Error in opening: %s" << filename << std::endl;
		exit(1);
	}
	
	// setAddr i2c slaves;
	if (ioctl(fd, I2C_SLAVE, addrFL) < 0) 
	{
		std::cout << "Error in connecting with sensor Front Left" << std::endl;
		exit(1);
	}

	
	
};


DistanceSensor::~DistanceSensor()
{
	close(fd);
}


char getDistance()
{
	//int read()
}