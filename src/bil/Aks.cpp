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
    while(1){
		UserInput latestUserInput;
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
				//Read data from all prox. sensors
				break;
			fwd:
				//read data from front prox. sensors
				break;
			bwd:
				//read data from rear prox. sensors
				break;
			default:
				break;
		}
		
		//read accell
		//if(analyzeData())
		//parse brake to steering
		//else
		//parse userinput to steering
		
	
    }
}