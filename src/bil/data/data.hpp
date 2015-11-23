#include <string>


class Data {
    
private:
    int distanceFL;
    int distanceFR;
    int distanceRL;
    int distanceRR;
    int acceleration;
    int velocity;
    
    pthread_mutex_t sensorDataMut;
    pthread_mutex_t userDataMut;
    
    pthread_cond_t sensorDataCond;
    pthread_cond_t userDataCond;
    
public:
    void writeVelocity(int velocity);
    void writeAcceleration(int acceleration);
    void writeDistance(std::string name, int distance);
    int getLatestVelocity();
    int getLatestAcceleration();
    int getLatestDistance(std::string name);
    Data();
    
};