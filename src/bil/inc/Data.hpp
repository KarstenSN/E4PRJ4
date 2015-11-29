#ifndef _INCL_DATA_HPP
#define _INCL_DATA_HPP

#include <string>
#include <mutex>
#include "utilities.hpp"
#include "Log.hpp"

class Data {
public:
    Data(Log* Log);
    ~Data();
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
    Log* Log_;
    
    std::mutex sensorDataMut;
    std::mutex userDataMut;
};

#endif