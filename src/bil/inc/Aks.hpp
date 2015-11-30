#ifndef AKS_HPP
#define AKS_HPP

#include <Data.hpp>
#include <Steering.hpp>
#include <Settings.hpp>
#include <Log.hpp>

#define NBR_PROX_SENSORS 4
#define DELTA_DISTANCE_FOR_ERROR 10

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
    Aks(Data*, Settings*, Log*);
	/*Activates the AKS, which enables steering of the car. Loops forever! */
    void activate(void);
	
private:
	bool analyzeData(void);
	
    Data* MyData;
    Settings* MySettings;
    Steering MySteering;
	Log* MyLog;
    aksStates state;
	int proxSensors[NBR_PROX_SENSORS];
	int old_proxSensors[NBR_PROX_SENSORS];
	UserInput latestUserInput;
};

#endif