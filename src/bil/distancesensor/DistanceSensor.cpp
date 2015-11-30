//DistanceSensor.cpp

#include <string>
#include "DistanceSensor.hpp"

int adapter_nr = 1;                             // Valg af I2C-bus
char filename[20];
int fd= 0;

// Constructor
DistanceSensor::DistanceSensor(Log* log){}

// Close Device()
DistanceSensor::~DistanceSensor(){}


// getDistance()
int DistanceSensor::getDistance(std::string name){
// open i2c device;
    int distanceSlave = 0x08;
    int rdBuffer[8];
    int FL = 0;
    int FR = 0;
    int RL = 0;
    int RR = 0;

    if (fd = open("/dev/i2c-1", O_RDWR) < 0){
        std::cout << "Error in opening i2c-bus " << std::endl;
        return -1;
    }

// Sæt adresse på sensor:
    // FrontLeft
    if (ioctl(fd, I2C_SLAVE, distanceSlave) < 0){
        std::cout << "Error in setting addr: " << distanceSlave << std::endl;
        return -1;
    }

    if (read(fd,rdBuffer,8) != 8){
        std::cout << "Failed to read distancesensor from I2C bus.\n" << std::endl;
        return -1;
    }
    
        close(fd);
    
    switch (name){
        case "FL":
            FL = (rdBuffer[0] << 8) + rdBuffer[1];
            return FL;
            break;
        case "FR":
            FR = (rdBuffer[2] << 8) + rdBuffer[3];
            return FR;
            break;
        case "RL":
            RL = (rdBuffer[4] << 8) + rdBuffer[5];
            return RL;
            break;
        case "RR":
            RR = (rdBuffer[6] << 8) + rdBuffer[7];
            return RR;
            break;
        default:
            std::cout << "Wrong parameter to getDistance" << std::endl;
            break;
    }
}

    // skriv i Log


/* Syntax til Log-klassen
*
* Log->writeEvent( __PRETTY_FUNCTION__ , msg (string))
*
* Event / Error / Warning
*/
