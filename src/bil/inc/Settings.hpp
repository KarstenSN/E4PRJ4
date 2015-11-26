#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <iostream>

//This is a dummy class, use only for testing!

class Settings{
public:
    Settings(){
        maxVelocity_ = 0;
        AKSStatus_ = 0;
        calibration_ = 0;
    }

    void setMaxSpeed(int maxVelocity){
        this->maxVelocity_ = maxVelocity;
        return;
    }

    void setAKS(bool AKSStatus){
        this->AKSStatus_ = AKSStatus;
        return;
    }
    
    bool getAKS(){
		return this->AKSStatus_;
	}

    void calibrateSteering(char calibration){
        this->calibration_ = calibration;
        return;
    }

private:
    int maxVelocity_;
    bool AKSStatus_;
    char calibration_;
};

#endif // SETTINGS_HPP
