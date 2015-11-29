#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <iostream>
#include <thread>
#include <mutex>

class Settings{
public:
    Settings();
    void setMaxSpeed(int maxVelocity);
    void setAKS(bool AKSStatus);
    void calibrateSteering(char calibration);
    int getMaxSpeed();
    bool getAKS();
    char getCalibration();

private:
    int maxVelocity_;
    bool AKSStatus_;
    char calibration_;
    
    std::mutex maxVelocityMut;
    std::mutex AKSStatusMut;
    std::mutex calibrationMut;   
};

#endif // SETTINGS_HPP
