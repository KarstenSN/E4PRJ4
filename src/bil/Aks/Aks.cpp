#include <Aks.hpp>
#include <utilities.hpp>
#include <Data.hpp>
#include <Settings.hpp>
#include <Steering.hpp>
#include <Log.hpp>

#define STILL_THRESH 20 //in km/h * 10
//#define DEBUGGING //Uncomment to enable debugging
#ifdef DEBUGGING
#include <iostream>
#endif


Aks::Aks(Data* Dataptr, Settings* NewSettings, Log* NewLog): MySteering(Dataptr, NewSettings, NewLog) {
	this->MyData = Dataptr;
    this->MySettings = NewSettings;
	this->MyLog = NewLog;
    this->state = still;
	MyLog->writeEvent(__PRETTY_FUNCTION__, "Initialized.");
}

void Aks::activate(){
	MyLog->writeEvent(__PRETTY_FUNCTION__, "Started - looping forever.");
	while(1){
		latestUserInput = this->MyData->getUserInput();
		if(this->MySettings->getAKS()){
			if(this->MyData->getLatestVelocity() < STILL_THRESH){
				this->state = still;
			}
			else{
				if(latestUserInput.forward < 10 && latestUserInput.reverse < 10){
					this->state = coasting;
				}
				else if(latestUserInput.forward > latestUserInput.reverse){
					this->state = fwd;
				}
				else{
					this->state = bwd;
				}
			}
		}
		else{
			this->state = still;
		}
		for(int i = 0; i < NBR_PROX_SENSORS; i++)
			old_proxSensors[i] = proxSensors[i];
        switch(this->state){
			case still:{
				this->MySteering.userInput(&latestUserInput);
				for(int i = 0; i < NBR_PROX_SENSORS; i++)
					proxSensors[i] = 0;
				break;
			}
			case coasting:
				//this should be replaced by a for loop if possible
				proxSensors[FR] = this->MyData->getLatestDistance("FR");
				proxSensors[FL] = this->MyData->getLatestDistance("FL");
				proxSensors[RR] = this->MyData->getLatestDistance("RR");
				proxSensors[RL] = this->MyData->getLatestDistance("RL");
				break;
			case fwd:
				proxSensors[FR] = this->MyData->getLatestDistance("FR");
				proxSensors[FL] = this->MyData->getLatestDistance("FL");
				proxSensors[RR] = 0;
				proxSensors[RL] = 0;
				break;
			case bwd:
				proxSensors[RR] = this->MyData->getLatestDistance("RR");
				proxSensors[RL] = this->MyData->getLatestDistance("RL");
				proxSensors[FR] = 0;
				proxSensors[FL] = 0;
				break;
			default:
				MyLog->writeWarning(__PRETTY_FUNCTION__, "Reached 'default' in switch statement");
				break;
		}
		if(analyzeData()){
#ifdef DEBUGGING 
			std::cout << "analyseData returned TRUE. "<< std::endl;
#endif
			UserInput tempUserInput;
			tempUserInput.stop = 1;
			MySteering.userInput(&tempUserInput);
		}
		else{
#ifdef DEBUGGING 
			std::cout << "analyseData returned FALSE. "<< std::endl;
#endif
			MySteering.userInput(&latestUserInput);
		}
    }
}

bool Aks::analyzeData(void){
#ifdef DEBUGGING 
	std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
	switch(this->state){
		case still:
			return false;
			break;
		case coasting:
			for(int i = 0; i < NBR_PROX_SENSORS; i++){
				if((old_proxSensors - proxSensors) > DELTA_DISTANCE_FOR_ERROR)
					return true;
			}
			break;
		case fwd:
			for(int i = FR; i <= FL; i++){
				if((old_proxSensors - proxSensors) > DELTA_DISTANCE_FOR_ERROR)
					return true;
			}
			break;
		case bwd:
			for(int i = RR; i <= RL; i++){
				if((old_proxSensors - proxSensors) > DELTA_DISTANCE_FOR_ERROR)
					return true;
			}
			break;
		default:
			MyLog->writeWarning(__PRETTY_FUNCTION__, "Reached 'default' in switch statement");
			break;
	}
	
	return false;
}
