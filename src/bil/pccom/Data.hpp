#ifndef DATA_HPP
#define DATA_HPP

#include <iostream>
#include <string>

class dataTest
{
public:
    dataTest()
    {
        velocity_ = 10;
        acceleration_ = 10;
        distance_ = 1;
    }

    int getLatestVelocity()
    {
        return this->velocity_;
    }

    int getLatestAcceleration()
    {
        return this->acceleration_;
    }

    int getLatestDistance(std::string name)
    {
        return this->distance_;
    }

private:
    int velocity_;
    int acceleration_;
    int distance_;
};

#endif // DATA_HPP
