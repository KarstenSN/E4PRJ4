#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>

class settingsTest
{
public:
    settingsTest()
    {
        maxVelocity_ = 0;
        AKSStatus_ = 0;
        calibration_ = 0;
    }

    void setMaxSpeed(int maxVelocity)
    {
        this->maxVelocity_ = maxVelocity;
        return;
    }

    void setAKS(bool AKSStatus)
    {
        this->AKSStatus_ = AKSStatus;
        return;
    }

    void calibrateSteering(char calibration)
    {
        this->calibration_ = calibration;
        return;
    }

private:
    int maxVelocity_;
    bool AKSStatus_;
    char calibration_;
};

#endif // SETTINGS_H
