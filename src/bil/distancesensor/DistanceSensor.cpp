//DistanceSensor.cpp
#include <DistanceSensor.hpp>

char filename[20];
int fd= 0;

// Constructor
DistanceSensor::DistanceSensor(Log* Log){
    this->Log_ = Log;
    std::cout << "Distance class running.." << std::endl;
    this->Log_->writeEvent(__PRETTY_FUNCTION__,"Distance class active");
}

// Close Device()
DistanceSensor::~DistanceSensor(){
    std::cout << "Distance class shutdown.." << std::endl;
    this->Log_->writeEvent(__PRETTY_FUNCTION__,"Data class shutdown");
}

// getDistance()
int DistanceSensor::getDistance(std::string name){
    std::cout << "getDistance() running.." << std::endl;
    this->Log_->writeEvent(__PRETTY_FUNCTION__,"getDistance() running");

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
    
    if( name == "FL" ){
        FL = (rdBuffer[0] << 8) + rdBuffer[1];
        std::cout << "FL distance returned: " << FL << std::endl;
        return FL;
    }
    else if( name == "FR" ){
        FR = (rdBuffer[2] << 8) + rdBuffer[3];
        std::cout << "FR distance returned: " << FR << std::endl;
        return FR;
    }
    else if( name == "RL" ){
        FR = (rdBuffer[2] << 8) + rdBuffer[3];
        std::cout << "RL distance returned: " << RL << std::endl;
        return RL;
    }
    else if( name == "RR" ){
        FR = (rdBuffer[2] << 8) + rdBuffer[3];
        std::cout << "RR distance returned: " << RR << std::endl;
        return RR;
    }
    else{
        std::cout << "Wrong parameter in getDistance" << std::endl;
    }
}