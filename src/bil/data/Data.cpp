#include <iostream>
#include <thread>
#include <Data.hpp>
#include <utilities.hpp>
#include <Log.hpp>

//----------Data::Data1----------
Data::Data(Log* myLog){
    this->myLog = myLog;
    this->distanceFL = 0;
    this->distanceFR = 0;
    this->distanceRL = 0;
    this->distanceRR = 0;
    this->acceleration = 0;
    this->velocity = 0;
    this->Input.forward = 0;
    this->Input.reverse = 0;
    this->Input.turn = 0;
    this->Input.stop = 0;
#ifdef DEBUG
    std::cout << "Data class running.." << std::endl;
#endif
    this->myLog->writeEvent(__PRETTY_FUNCTION__,"Data class active");
}
//----------Data::Data2----------

//----------Data::~Data1----------
Data::~Data(){
    this->myLog->writeEvent(__PRETTY_FUNCTION__,"Data class shutdown");
}
//----------Data::~Data2----------

//----------Data::writeVelocity1----------
bool Data::writeVelocity(int velocity){
    if(velocity >= 0 && velocity <= MAX_VELOCITY){
        std::lock_guard<std::mutex> lock(sensorDataMut);
        this->velocity = velocity;
        return true;
    }
    else return false;
}
//----------Data::writeVelocity2----------

//----------Data::writeAcceleration1----------
bool Data::writeAcceleration(int acceleration){
    if(acceleration >= 0 && acceleration <= MAX_ACCELERATION){
        std::lock_guard<std::mutex> lock(sensorDataMut);
        this->acceleration = acceleration;
        return true;
    }
    else return false;
}
//----------Data::writeAcceleration2----------

//----------Data::writeDistance1----------
bool Data::writeDistance(std::string name, int distance){
    if(distance >= 0 && distance <= MAX_DISTANCE){
        std::lock_guard<std::mutex> lock(sensorDataMut);
        if (name == "FL") {
            this->distanceFL = distance;
        }
        else if(name == "FR") {
            this->distanceFR = distance;
        }
        else if(name == "RL") {
            this->distanceRL = distance;
        }
        else if(name == "RR") {
            this->distanceRR = distance;
        }
        else{
#ifdef DEBUG
            std::cout << "name can only be: FL, FR, RL or RR" << std::endl;
#endif
            this->myLog->writeWarning(__PRETTY_FUNCTION__,"Error in input argument std::string name");
            return false;
        }
        return true;
    }
    else return false;
}
//----------Data::writeDistance2----------

//----------Data::writeUserInput1----------
void Data::writeUserInput(UserInput* Input){
    std::lock_guard<std::mutex> lock(userDataMut);
    this->Input = *Input;
}
//----------Data::writeUserInput2----------

//----------Data::getLatestVelocity1----------
int Data::getLatestVelocity(){
    std::lock_guard<std::mutex> lock(sensorDataMut);
    return this->velocity;
}
//----------Data::getLatestVelocity2----------

//----------Data::getLatestAcceleration1----------
int Data::getLatestAcceleration(){
    std::lock_guard<std::mutex> lock(sensorDataMut);
    return this->acceleration;
}
//----------Data::getLatestAcceleration2----------

//----------Data::getLatestDistance1----------
int Data::getLatestDistance(std::string name){
    std::lock_guard<std::mutex> lock(sensorDataMut);
    if(name == "FL") {
        return this->distanceFL;
    }
    else if(name == "FR") {
        return this->distanceFR;
    }
    else if(name == "RL") {
        return this->distanceRL;
    }
    else if(name == "RR") {
        return this->distanceRR;
    }
    else{
#ifdef DEBUG
        std::cout << "name can only be: FL, FR, RL or RR" << std::endl;
#endif
        this->myLog->writeWarning(__PRETTY_FUNCTION__,"Error in input argument std::string name");
        return -1;
    }
}
//----------Data::getLatestDistance2----------

//----------Data::getUserInput1----------
UserInput Data::getUserInput(){
    std::lock_guard<std::mutex> lock(userDataMut);
    return this->Input;
}
//----------Data::getUserInput2----------