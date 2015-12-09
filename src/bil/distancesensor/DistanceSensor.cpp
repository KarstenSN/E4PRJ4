//DistanceSensor.cpp
#include <DistanceSensor.hpp>

//----------DS::CON1----------
DistanceSensor::DistanceSensor(Log* Log){
    this->Log_ = Log;
    std::cout << "Distance class running.." << std::endl;
    this->Log_->writeEvent(__PRETTY_FUNCTION__,"Distance class active");

}
//----------DS::CON2----------

// Close Device()
DistanceSensor::~DistanceSensor(){
    std::cout << "Distance class shutdown.." << std::endl;
    this->Log_->writeEvent(__PRETTY_FUNCTION__,"Data class shutdown");
}

//----------DS::getD1----------
int DistanceSensor::getDistance(std::string name){
    //std::cout << "getDistance() running.." << std::endl;
    this->Log_->writeEvent(__PRETTY_FUNCTION__,"getDistance() running");

    // open i2c device;
    int fd;
    int distanceSlave = 0x10;
    char rdBuffer[9] = {0};

    if ((fd = open("/dev/i2c-1", O_RDWR)) < 0){
        std::cout << "Error in opening i2c-bus (DistanceSensor)" << std::endl;
        close(fd);
        return -1;
    }

    // Set sensoradresse:
    // FrontLeft
    if ((ioctl(fd, I2C_SLAVE, distanceSlave)) < 0){
        std::cout << "Error in setting addr: (DistanceSensor)" << distanceSlave << std::endl;
        close(fd);
        return -2;
    }

    int check = read(fd,rdBuffer,9);
    if (check != 9){
        std::cout << "Failed to read distancesensor from I2C bus.\n" << std::endl;
        close(fd);
        return -3;
    }
    
    close(fd);
    
    if( name == "FL" ){
        distanceFL_ = (int)((rdBuffer[0] << 8) + rdBuffer[1]);
        //std::cout << "FL distance returned: " << distanceFL_ << std::endl;
        return distanceFL_;
    }
    else if( name == "FR" ){
        distanceFR_ = (int)((rdBuffer[2] << 8) + rdBuffer[3]);
        //std::cout << "FR distance returned: " << distanceFR_ << std::endl;
        return distanceFR_;
    }
    else if( name == "RL" ){
        distanceRL_ = (int)((rdBuffer[4] << 8) + rdBuffer[5]);
        //std::cout << "RL distance returned: " << distanceRL_ << std::endl;
        return distanceRL_;
    }
    else if( name == "RR" ){
        distanceRR_ = (int)((rdBuffer[6] << 8) + rdBuffer[7]);
        //std::cout << "RR distance returned: " << distanceRR_ << std::endl;
        return distanceRR_;
    }
    else{
        std::cout << "Wrong parameter in getDistance " << std::endl;
        return 4;
    }    
}
//----------DS::getD2----------