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

int addrFL = 0b01110000;                // 0x70 dec 112
int addrFR = 0b01110001;                // 0x71 dec 113
int addrRL = 0b01110011;                // 0x73 dec 115
int addrRR = 0b01110110;                // 0x76 dec 118
int distanceFL = 0;	
int distanceFR = 0;
int distanceRL = 0;
int distanceRR = 0;

// MaxSonar1202 commands:
int FLwrite = (addrFL << 1) & 11111110;  // adressen sættes til write-enable
int FLread  = (addrFL << 1) + 1;         // adressen sættes til read-enable
int FRwrite = (addrFR << 1) & 11111110;
int FRread  = (addrFR << 1) + 1;
int RLwrite = (addrRL << 1) & 11111110;
int RLread  = (addrRL << 1) + 1;
int RRwrite = (addrRR << 1) & 11111110;
int RRread  = (addrRR << 1) + 1;

int fd = 0;
int adapter_nr = 1;                      // Valg af I2C-bus
char filename[20];


// Opning af I2C bussen
DistanceSensor::DistanceSensor()
{
	// open i2c device;
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
    std::cout << "opening: "<< filename << std::endl;

    if (int fd = open(filename, O_RDWR) < 0)
    {
        std::cout << "Error in opening: %s" << filename << std::endl;
        exit(1);
    }

    // Sæt adresse på sensor
    if (ioctl(fd, I2C_SLAVE, addrFL) < 0)
    {
        std::cout << "Error in setting addr: %s" << filename << std::endl;
        exit(1);
    }
}


// Close Device
DistanceSensor::~DistanceSensor()
{
	close(fd);
}


/*char getDistance()
{
        int data[0] = FRwrite;
        if (write(fd, data, 1) !=1 )
        {
            std::cout << "Error CMDwriting to: %s" << filename << std::endl;
        }


        //int read()


    return 0;
}
*/






/* Syntax til Log-klassen
*
* Log->writeEvent(from (string), msg (string))
*
* Event / Error / Warning
*/
