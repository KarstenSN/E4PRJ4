#ifndef _INCL_DATA_HPP
#define _INCL_DATA_HPP

#include <string>
#include <mutex>
#include <utilities.hpp>
#include <Log.hpp>

#define MAX_VELOCITY 200        // (km/t)*10
#define MAX_ACCELERATION 200    // G*10
#define MAX_DISTANCE 250        // m*10



class Data {
public:
    Data(Log* myLog);
    ~Data();
    bool writeVelocity(int velocity);
    bool writeAcceleration(int acceleration);
    bool writeDistance(std::string Name, int distance);
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
    Log* myLog;
    
    std::mutex sensorDataMut_vel;
    std::mutex sensorDataMut_acc;
    std::mutex sensorDataMut_distFL;
    std::mutex sensorDataMut_distFR;
    std::mutex sensorDataMut_distRL;
    std::mutex sensorDataMut_distRR;
    std::mutex userDataMut;
};

#endif