//Psoc.cpp
#include <Psoc.hpp>

char rdBuffer[9] = {0};

// Constructor
Psoc::Psoc(Log* Log){
    this->logPtr_ = Log;
    std::cout << "Psoc class running.." << std::endl;
    this->logPtr_->writeEvent(__PRETTY_FUNCTION__,"Psoc class active");
    
    this->psocThread_ = std::thread(&Psoc::psocThread_ , this);
}


// Destructor
Psoc::~Psoc(){
    std::cout << "Psoc class shutdown.." << std::endl;
    this->logPtr_->writeEvent(__PRETTY_FUNCTION__,"Psoc class shutdown");
    this->psocThread_.join();
}


// getDistance()
int Psoc::getDistance(std::string name){
    
    std::lock_guard<std::mutex> lock(psocMut);
    
    std::cout << "getdistance() kører" << std::endl;
    
    if( name == "FL" ){
        return distanceFL_;
    }
    else if( name == "FR" ){
        return distanceFR_;
    }
    else if( name == "RL" ){
        return distanceRL_;
    }
    else if( name == "RR" ){
        return distanceRR_;
    }
    else{
        std::cout << "Wrong parameter in getDistance " << std::endl;
        return 4;
    }
}


// getVelocity()
int Psoc::getVelocity(){
    
    std::cout << "getVelocity() kører" << std::endl;
  
    return Tacho_;
}


// PsocRead()
void Psoc::psocRead(){
    
    std::cout << "test af Thread" << std::endl;
    
    while(1){
	  // get data from PSOC
	  Psoc::update();
	
	  std::lock_guard<std::mutex> lock(psocMut);
	  
	  // move data to members
	  distanceFL_ = (int)((rdBuffer[0] << 8) + rdBuffer[1]);
	  distanceFR_ = (int)((rdBuffer[2] << 8) + rdBuffer[3]);
	  distanceRL_ = (int)((rdBuffer[4] << 8) + rdBuffer[5]);
	  distanceRR_ = (int)((rdBuffer[6] << 8) + rdBuffer[7]);
	  Tacho_      = (int)(rdBuffer[8]);
	
	  std::this_thread::sleep_for(std::chrono::seconds(1));
      }
}


// update()
int Psoc::update(){
    
    // open i2c device;
    int fd;
    int Psocslave = 0x10;
    
    std::cout << "Update() kører" << std::endl;

    if ((fd = open("/dev/i2c-1", O_RDWR)) < 0){
        std::cout << "Error in opening I2C-Bus (DistanceSensor)" << std::endl;
        close(fd);
        return -1;
    }

    // Sæt adresse på sensor:
    if ((ioctl(fd, I2C_SLAVE, Psocslave)) < 0){
        std::cout << "Error in setting addr: (DistanceSensor)" << Psocslave << std::endl;
        close(fd);
        return -2;
    }   
    // Læsning og fejlcheck
    int check = read(fd,rdBuffer,9);
    if (check != 9){
        std::cout << "Failed to read distancesensor from I2C bus.\n" << std::endl;
        close(fd);
        return -3;
    }
    close(fd);
}