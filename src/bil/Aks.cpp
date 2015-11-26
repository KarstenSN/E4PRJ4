#include <Aks.hpp>
#include <utilities.hpp>
#include <Data.hpp>
#include <Settings.hpp>
#include <Steering.hpp>

#define STILL_THRESH 20 //in km/h * 10


Aks::Aks(Data* Dataptr, Settings* NewSettings): MySteering(Dataptr, NewSettings) {
    this->MyData = Dataptr;
    this->MySettings = NewSettings;
    state = still;
}

void Aks::activate(){
	UserInput latestUserInput;
    while(1){
		if(this->MySettings->getAKS()){
			if(this->MyData->getLatestVelocity() < STILL_THRESH){
				this->state = still;
			}
			else{
				latestUserInput = this->MyData->getUserInput();
			}
		}
		else{
			state = still;
			latestUserInput = this->MyData->getUserInput();
		}
        switch(this->state){
			still:
				this->MySteering.userInput(&latestUserInput);
				break;
			coasting:
				//this should be replaced by a for loop if possible
				proxSensors[FR] = this->MyData->getLatestDistance("FR");
				proxSensors[FL] = this->MyData->getLatestDistance("FL");
				proxSensors[RR] = this->MyData->getLatestDistance("RR");
				proxSensors[RL] = this->MyData->getLatestDistance("RL");
				break;
			fwd:
				proxSensors[FR] = this->MyData->getLatestDistance("FR");
				proxSensors[FL] = this->MyData->getLatestDistance("FL");
				break;
			bwd:
				proxSensors[RR] = this->MyData->getLatestDistance("RR");
				proxSensors[RL] = this->MyData->getLatestDistance("RL");
				break;
			default:
				break;
		}
		
		this->currentAccell = this->MyData->getLatestAcceleration();
		if(analyzeData()){
			UserInput tempUserInput;
			tempUserInput.stop = 1;
			MySteering.userInput(&tempUserInput);
		}
		else
			MySteering.userInput(&latestUserInput);	
    }
}

//TODO Make this function yay
bool Aks::analyzeData(void){
	
	return false;
}
