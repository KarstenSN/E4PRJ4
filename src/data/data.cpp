#include <iostream>
#include "data.hpp"

Data::Data(){
    std::cout << "data class running.." << std::endl;
    this->distanceFL = 0;
    this->distanceFR = 0;
    this->distanceRL = 0;
    this->distanceRR = 0;
    this->acceleration = 0;
    this->velocity = 0;
}

void Data::writeVelocity(int velocity){
    this->velocity = velocity;
}

void Data::writeAcceleration(int acceleration){
    this->acceleration = acceleration;
}

void Data::writeDistance(std::string name, int distance){
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

int Data::getLatestVelocity(){
    return this->velocity;
}

int Data::getLatestAcceleration(){
    return this->acceleration;
}

int Data::getLatestDistance(std::string name){
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