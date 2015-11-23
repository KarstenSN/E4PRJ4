#include <iostream>
#include <pthread.h>
#include <Data.hpp>
#include <utilities.hpp>

Data::Data(){
    std::cout << "data class running.." << std::endl;
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
}

void Data::writeVelocity(int velocity){
    ScopedLocker Lock(&sensorDataMut);
    this->velocity = velocity;
}

void Data::writeAcceleration(int acceleration){
    ScopedLocker Lock(&sensorDataMut);
    this->acceleration = acceleration;
}

void Data::writeDistance(std::string name, int distance){
    ScopedLocker Lock(&sensorDataMut);
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
        std::cout << "name can only be: FL, FR, RL or RR" << std::endl;
    }
}

void Data::writeUserInput(UserInput Input){
    ScopedLocker Lock(& userDataMut);
    this->Input = Input;
}

int Data::getLatestVelocity(){
    ScopedLocker Lock(&sensorDataMut);
    return this->velocity;
}

int Data::getLatestAcceleration(){
    ScopedLocker Lock(&sensorDataMut);
    return this->acceleration;
}

int Data::getLatestDistance(std::string name){
    ScopedLocker Lock(&sensorDataMut);
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
        std::cout << "name can only be: FL, FR, RL or RR" << std::endl;
        return -1;
    }
}

userInput Data::getUserInput(){
    ScopedLocker Lock(&userDataMut);
    return this->Input;
}
