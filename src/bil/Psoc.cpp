//Psoc.cpp
#include <Psoc.hpp>

//#define DEBUGGING_MODE

char rdBuffer[9] = {0};

//----------psoc::ctor1----------
Psoc::Psoc(Log* Log){
	this->logPtr_ = Log;
	#ifdef DEBUGGING_MODE
	std::cout << "Psoc class running.." << std::endl;
	#endif 
	this->logPtr_->writeEvent(__PRETTY_FUNCTION__,"Psoc class active");
    
	this->psocThread_ = std::thread(&Psoc::psocRead , this);
}
//----------psoc::ctor2----------

//----------psoc::dtor1----------
Psoc::~Psoc(){
	#ifdef DEBUGGING_MODE
	std::cout << "Psoc class shutdown.." << std::endl;
	#endif 
	this->logPtr_->writeEvent(__PRETTY_FUNCTION__,"Psoc class shutdown");
	this->psocThread_.join();
}
//----------psoc::dtor2----------


//----------psoc::getDistance1----------
int Psoc::getDistance(std::string name){
    
	#ifdef DEBUGGING_MODE
    std::cout << "getdistance() running" << std::endl;
	#endif 
    std::lock_guard<std::mutex> lock(psocMut);
    
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
          this->logPtr_->writeWarning(__PRETTY_FUNCTION__,"Psoc class shutdown");
        std::cout << "Wrong parameter in getDistance " << std::endl;
        return 4;
    }
}
//----------psoc::getDistance2----------


//----------psoc::getVelocity1----------
int Psoc::getVelocity(){
	#ifdef DEBUGGING_MODE
    std::cout << "getVelocity() kører" << std::endl;
	#endif
  
	std::lock_guard<std::mutex> lock(psocMut);
	
    return Tacho_;
}
//----------psoc::getVelocity2----------


//----------psoc::read1----------
void Psoc::psocRead(){
	#ifdef DEBUGGING_MODE    
	std::cout << "test af Thread" << std::endl;
	#endif     
	while(1){
	// get data from PSOC
	Psoc::update();
	{
		std::lock_guard<std::mutex> lock(psocMut);
	  
		// move data to members
		distanceFL_ = (int)((rdBuffer[0] << 8) + rdBuffer[1]);
		distanceFR_ = (int)((rdBuffer[2] << 8) + rdBuffer[3]);
		distanceRL_ = (int)((rdBuffer[4] << 8) + rdBuffer[5]);
		distanceRR_ = (int)((rdBuffer[6] << 8) + rdBuffer[7]);
		Tacho_      = (int)(rdBuffer[8]);
	}
	#ifdef DEBUGGING_MODEs
	std::cout << "Fl: " << distanceFL_ << std::endl;
	std::cout << "FR: " << distanceFR_ << std::endl;
	std::cout << "RL: " << distanceRL_ << std::endl;
	std::cout << "RR: " << distanceRR_ << std::endl;
	#endif
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}
//----------psoc::read2----------

//----------psoc::update1----------
int Psoc::update(){
    
	// open i2c device;
	int fd;
	int Psocslave = 0x10;
	#ifdef DEBUGGING_MODE
	std::cout << "Update() kører" << std::endl;
	#endif

	if ((fd = open("/dev/i2c-1", O_RDWR)) < 0){
		this->logPtr_->writeError(__PRETTY_FUNCTION__,"Error opening I2C-bus");
		#ifdef DEBUGGING_MODE
		std::cout << "Error in opening I2C-Bus (DistanceSensor)" << std::endl;
		#endif
		close(fd);
		return -1;
	}

	// Sæt adresse på sensor:
	if ((ioctl(fd, I2C_SLAVE, Psocslave)) < 0){
		this->logPtr_->writeError(__PRETTY_FUNCTION__,"Comm error via I2C.");
		#ifdef DEBUGGING_MODE
		std::cout << "Error in setting addr: (DistanceSensor)" << Psocslave << std::endl;
		#endif
		close(fd);
		return -2;
	}   
	// Læsning og fejlcheck
	int check = read(fd,rdBuffer,9);
	if (check != 9){
		#ifdef DEBUGGING_MODE
		std::cout << "Failed to read distancesensor from I2C bus.\n" << std::endl;
		#endif
		this->logPtr_->writeError(__PRETTY_FUNCTION__,"Failed to read psoc from I2C bus.");
		close(fd);
		return -3;
	}
	close(fd);
}
//----------psoc::update2----------
