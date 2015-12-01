//DistanceSensor.cpp
#include <DistanceSensor.hpp>

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
    //std::cout << "getDistance() running.." << std::endl;
    this->Log_->writeEvent(__PRETTY_FUNCTION__,"getDistance() running");

    // open i2c device;
    int fd;
    int distanceSlave = 0x10;
    int rdBuffer[9];
    int FL = 0;
    int FR = 0;
    int RL = 0;
    int RR = 0;

    if ((fd = open("/dev/i2c-1", O_RDWR)) < 0){
        std::cout << "Error in opening i2c-bus " << std::endl;
        return 1;
    }

    // Sæt adresse på sensor:
    // FrontLeft
    if ((ioctl(fd, I2C_SLAVE, distanceSlave)) < 0){
        std::cout << "Error in setting addr: " << distanceSlave << std::endl;
        return 2;
    }

    if ((read(fd,rdBuffer,9)) != 9){
        std::cout << "Failed to read distancesensor from I2C bus.\n" << std::endl;
        return 3;
    }
    
    std::cout << "#0: " << rdBuffer[0] << "\n" <<  "#1: " << rdBuffer[1] << "\n" << "#2: " << rdBuffer[2] << "\n" << "#3: " << rdBuffer[3] << "\n" << "#4: " << rdBuffer[4] << "\n" << "#5: " << rdBuffer[5] << "\n" << "#6: "  << rdBuffer[6] << "\n" << "#7: "  << rdBuffer[7] << "\n\n\n\n" <<std::endl; 
    
    if( name == "FL" ){
        FL = ((rdBuffer[0] << 8) + rdBuffer[1]);
        //std::cout << "FL distance returned: " << FL << std::endl;
        return FL;
    }
    else if( name == "FR" ){
        FR = ((rdBuffer[2] << 8) + rdBuffer[3]);
        //std::cout << "FR distance returned: " << FR << std::endl;
        return FR;
    }
    else if( name == "RL" ){
        RL = ((rdBuffer[4] << 8) + rdBuffer[5]);
       // std::cout << "RL distance returned: " << RL << std::endl;
        return RL;
    }
    else if( name == "RR" ){
        RR = ((rdBuffer[6] << 8) + rdBuffer[7]);
        //std::cout << "RR distance returned: " << RR << std::endl;
        return RR;
    }
    else{
        std::cout << "Wrong parameter in getDistance" << std::endl;
        return 4;
    }
    close(fd);
}