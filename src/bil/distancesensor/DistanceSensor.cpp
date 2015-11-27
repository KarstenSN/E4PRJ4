//DistanceSensor.cpp

#include "DistanceSensor.hpp"
#include <linux/i2c-dev.h>
//#include "Log.hpp"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int fd = 0;
int adapter_nr = 1;                             // Valg af I2C-bus
char filename[20];

// Constructor
DistanceSensor::DistanceSensor()
{
    // Initialisering af adresser
    this->addrFL = 0b01110000;                  // 0x70 dec 112
    this->addrFR = 0b01110001;                  // 0x71 dec 113
    this->addrRL = 0b01110011;                  // 0x73 dec 115
    this->addrRR = 0b01110110;                  // 0x76 dec 118
    
    // Initealisering af afstande
    this->distanceFL = 0;
    this->distanceFR = 0;
    this->distanceRL = 0;
    this->distanceRR = 0;    
    
    // open i2c device;
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
    std::cout << "opening: "<< filename << std::endl;

    if (int fd = open(filename, O_RDWR) < 0)
    {
        std::cout << "Error in opening: %s" << filename << std::endl;
        exit(1);
    }

    // Sæt adresse på sensor
    // FrontLeft
    if (ioctl(fd, I2C_SLAVE, addrFL) < 0)
    {
        std::cout << "Error in setting addr: %d" << addrFL << std::endl;
        exit(1);
    }
    
    // FrontRight
    if (ioctl(fd, I2C_SLAVE, addrFR) < 0)
    {
        std::cout << "Error in setting addr: %d" << addrFR << std::endl;
        exit(1);
    }
    // RearLeft
    if (ioctl(fd, I2C_SLAVE, addrRL) < 0)
    {
        std::cout << "Error in setting addr: %d" << addrRL << std::endl;
        exit(1);
    }
    // RearRight
    if (ioctl(fd, I2C_SLAVE, addrRR) < 0)
    {
        std::cout << "Error in setting addr: %d" << addrRR << std::endl;
        exit(1);
    }
}


// Close Device
DistanceSensor::~DistanceSensor()
{
	close(fd);
}


char DistanceSensor::getDistance(std::string name)
{
    // MaxSonar1202 commands:
    int FLwrite = (this->addrFL << 1) & 11111110;     // adressen sættes til write-enable
    int FLread  = (this->addrFL << 1) + 1;            // adressen sættes til read-enable
    int FRwrite = (this->addrFR << 1) & 11111110;
    int FRread  = (this->addrFR << 1) + 1;
    int RLwrite = (this->addrRL << 1) & 11111110;
    int RLread  = (this->addrRL << 1) + 1;
    int RRwrite = (this->addrRR << 1) & 11111110;
    int RRread  = (this->addrRR << 1) + 1;    
    
    //int write()

    //int read()

    return 0;
}


/* Syntax til Log-klassen
*
* Log->writeEvent(from (string), msg (string))
*
* Event / Error / Warning
*/
