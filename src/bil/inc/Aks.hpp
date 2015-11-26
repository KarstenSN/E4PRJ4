#ifndef AKS_HPP
#define AKS_HPP

#include <Data.hpp>
#include <Steering.hpp>
#include <Settings.hpp>

#define NBR_PROX_SENSORS

enum aksStates{
    fwd,
    bwd,
    coasting,
    still
};

enum sensorsPos{
	FR = 0,
	FL = 1,
	RR = 2,
	RL = 3
};

class Aks{
public:
    Aks(Data*, Settings*);
	/*Activates the AKS, which enables steering of the car. Loops forever! */
    void activate(void);
	
private:
	bool analyzeData(void);
	
    Data* MyData;
    Settings* MySettings;
    Steering MySteering;
    aksStates state;
	int proxSensors[NBR_PROX_SENSORS];
	int currentAccell;
};


#endif