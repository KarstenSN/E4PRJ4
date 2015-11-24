#ifndef Steering_HPP
#define Steering_HPP

#include <Settings.hpp>

//This is a dummy class

class Steering{
public:
    Steering(Settings* NewSettings){
        this->MySettings = NewSettings;
    }
private:
    Settings* MySettings;
};

#endif