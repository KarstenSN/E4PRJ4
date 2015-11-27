#include <iostream>
#include <pthread.h>
#include <Data.hpp>
#include <PcCom.hpp>
#include <Aks.hpp>
#include <Settings.hpp>
#include <Steering.hpp>
#include <Log.hpp>
#include <DistanceSensor.hpp>

pthread_t acqDataThread;

struct ObjPts{
	Data* dataPtr;
	DistanceSensor* proxSensPtr;
	//TachoMeter* tachoPtr;
	//Accellerometer* AccellPtr;
	Log* LogPtr;
};

void* acquireData(void* structPtr){
	Data* myData = static_cast<ObjPts*>(structPtr)->dataPtr;
	DistanceSensor* myDistSens = static_cast<ObjPts*>(structPtr)->proxSensPtr;
	//TachoMeter* myTacho = static_cast<ObjPts*>(structPtr)->tachoPtr;
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
		
		//myData->writeVelocity(myTacho->somethingsoemthing);
		//TODO: make sure this function always loops the same amount of times/sec
		sleep(1);
	}
}

int main(){
    Settings Settings_;
	Log Log_("logfile.txt");
    Data Data_(&Log_);
    Aks Aks_(&Data_, &Settings_, &Log_);
    PcCom PcCom_(&Data_, &Settings_, &Log_);
	
	ObjPts AcquireObjects;
	AcquireObjects.dataPtr = &Data_;
	AcquireObjects.LogPtr = &Log_;
	
	pthread_create(&acqDataThread, NULL, acquireData, static_cast<void*>(&AcquireObjects));
    
	Log_.writeEvent("pi.cpp", "System initialized.");
    Aks_.activate();
    
    return 0;
}