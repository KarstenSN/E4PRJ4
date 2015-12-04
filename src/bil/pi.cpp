#include <iostream>
#include <thread>
#include <chrono>
#include <Data.hpp>
#include <PcCom.hpp>
#include <Aks.hpp>
#include <Settings.hpp>
#include <Steering.hpp>
#include <Log.hpp>
#include <DistanceSensor.hpp>
#include <Tachometer.hpp>

//#define DEBUGGING //Uncomment to enable debugging

struct ObjPts{
	Data* dataPtr;
	DistanceSensor* proxSensPtr;
	Tachometer* tachoPtr;
	//Accellerometer* AccellPtr;
	Log* LogPtr;
};

void* acquireData(ObjPts* structPtr){
	Data* myData = static_cast<ObjPts*>(structPtr)->dataPtr;
	DistanceSensor* myDistSens = static_cast<ObjPts*>(structPtr)->proxSensPtr;
	Tachometer* myTacho = static_cast<ObjPts*>(structPtr)->tachoPtr;
	//Accellerometer* myAccel = static_cast<ObjPts*>(structPtr)->AccellPtr;
	Log* myLog = static_cast<ObjPts*>(structPtr)->LogPtr;
	std::string sensors[4] = {"FR", "FL", "RR", "RL"};
	myLog->writeEvent(__PRETTY_FUNCTION__, "Initialized");
	while(1){
		//myData->writeAcceleration();
		for(int i = 0; i < NBR_PROX_SENSORS; i++){
			char temp = myDistSens->getDistance(sensors[i]);
			myData->writeDistance(sensors[i], temp);
			if(temp < 0){
				myLog->writeError(__PRETTY_FUNCTION__, "Negative return value from DistanceSensor: " + sensors[i]);
			}
		}
		char temp = myTacho->getVelocity();
		myData->writeVelocity(temp);
		if(temp < 0){
			myLog->writeError(__PRETTY_FUNCTION__, "Negative return value from Tachometer");
		}
		//TODO: make sure this function always loops the same amount of times/sec
		std::this_thread::sleep_for (std::chrono::seconds(1));
	}
}

int main(){
	Log Log_;
    Settings Settings_(&Log_);
    Data Data_(&Log_);
    Aks Aks_(&Data_, &Settings_, &Log_);
    PcCom PcCom_(&Data_, &Settings_, &Log_);
	Tachometer Tacho_(&Log_);
	
	ObjPts AcquireObjects;
	AcquireObjects.dataPtr = &Data_;
	AcquireObjects.LogPtr = &Log_;
	AcquireObjects.tachoPtr = &Tacho_;
	
	std::thread acqDataThread(acquireData, &AcquireObjects);
	
	Log_.writeEvent(__PRETTY_FUNCTION__, "System initialized.");
    Aks_.activate();
	
	acqDataThread.join();
    
    return 0;
}