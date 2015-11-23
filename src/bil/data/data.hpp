#include <string>


class Data {
    
private:
    int distanceFL;
    int distanceFR;
    int distanceRL;
    int distanceRR;
    int acceleration;
    int velocity;
    
public:
    void writeVelocity(int velocity);
    void writeAcceleration(int acceleration);
    void writeDistance(std::string name, int distance);
    int getLatestVelocity();
    int getLatestAcceleration();
    int getLatestDistance(std::string name);
    Data();
    
};
