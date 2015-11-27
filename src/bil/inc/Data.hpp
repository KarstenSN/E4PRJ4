#ifndef _INCL_DATA_HPP
#define _INCL_DATA_HPP

#include <string>
#include <utilities.hpp>
#include <Log.hpp>

class Data {
public:
    Data(Log*);
    void writeVelocity(int velocity);
    void writeAcceleration(int acceleration);
    void writeDistance(std::string Name, int distance);
    void writeUserInput(UserInput* Input);
    int getLatestVelocity();
    int getLatestAcceleration();
    int getLatestDistance(std::string name);
    UserInput getUserInput();
    
private:
    int distanceFL;
    int distanceFR;
    int distanceRL;
    int distanceRR;
    int acceleration;
    int velocity;
    UserInput Input;
	Log* MyLog;
    
    pthread_mutex_t sensorDataMut;
    pthread_mutex_t userDataMut;
    
    pthread_cond_t sensorDataCond;
    pthread_cond_t userDataCond;

};

#endif