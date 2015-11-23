#ifndef _INCL_DATA_HPP
#define _INCL_DATA_HPP

#include <string>
#include "utilities.hpp"

class Data {
    
private:
    int distanceFL;
    int distanceFR;
    int distanceRL;
    int distanceRR;
    int acceleration;
    int velocity;
    userInput input;
    
    pthread_mutex_t sensorDataMut;
    pthread_mutex_t userDataMut;
    
    pthread_cond_t sensorDataCond;
    pthread_cond_t userDataCond;
    
public:
    void writeVelocity(int velocity);
    void writeAcceleration(int acceleration);
    void writeDistance(std::string name, int distance);
    void writeUserInput(userInput input);
    int getLatestVelocity();
    int getLatestAcceleration();
    int getLatestDistance(std::string name);
    userInput getUserInput();
    Data();
    
};

#endif