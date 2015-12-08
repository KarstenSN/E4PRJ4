#include <iostream>
#include <thread>
#include <chrono>
#include <Data.hpp>
#include <PcCom.hpp>
#include <Aks.hpp>
#include <Settings.hpp>
#include <Steering.hpp>
#include <Log.hpp>
#include <Psoc.hpp>

//#define DEBUGGING //Uncomment to enable debugging

struct ObjPts{
	Data* dataPtr;
	Psoc* PsocPtr;
	//Accellerometer* AccellPtr;
	Log* LogPtr;
};

void* acquireData(ObjPts* structPtr){
	Data* myData = static_cast<ObjPts*>(structPtr)->dataPtr;
	Psoc* PsocSens = static_cast<ObjPts*>(structPtr)->PsocPtr;
	//Accellerometer* myAccel = static_cast<ObjPts*>(structPtr)->AccellPtr;
	Log* myLog = static_cast<ObjPts*>(structPtr)->LogPtr;
	std::string sensors[4] = {"FR", "FL", "RR", "RL"};
	myLog->writeEvent(__PRETTY_FUNCTION__, "Initialized");
	while(1){
		//myData->writeAcceleration();
		for(int i = 0; i < NBR_PROX_SENSORS; i++){
			int temp = PsocSens->getDistance(sensors[i]);
			myData->writeDistance(sensors[i], temp);
			if(temp < 0){
				myLog->writeError(__PRETTY_FUNCTION__, "Negative return value from Psoc_distancesensor: " + sensors[i]);
			}
		}
		int temp = PsocSens->getVelocity();
		myData->writeVelocity(temp);
		if(temp < 0){
			myLog->writeError(__PRETTY_FUNCTION__, "Negative return value from Psoc_tachometer");
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
	Psoc Psoc_(&Log_);
	
	ObjPts AcquireObjects;
	AcquireObjects.dataPtr = &Data_;
	AcquireObjects.LogPtr = &Log_;
	AcquireObjects.PsocPtr = &Psoc_;
	std::thread acqDataThread(acquireData, &AcquireObjects);
	
	Log_.writeEvent(__PRETTY_FUNCTION__, "System initialized.");
        Aks_.activate();
	
	acqDataThread.join();
    
    return 0;
}