#include "Tachometer.hpp"
#include "DistanceSensor.hpp"

int main() {
    
    Log myLog;
    
    Tachometer myTacho(&myLog);
    DistanceSensor myDist(&myLog);
    
    char p = 'X';
    int FL = 0;
    int FR = 0;
    int RL = 0;
    int RR = 0;
    
    while (1){
    FL = myDist.getDistance("FL");
    //sleep(100);
    FR = myDist.getDistance("FR");
    //sleep(100);
    RL = myDist.getDistance("RL");
    //sleep(100);
    RR = myDist.getDistance("RR");
    //sleep(100);
    p = myTacho.getVelocity();
    //sleep(100);
    std::cout << "FL: " << FL << std::endl;
    std::cout << "FR: " << FR << std::endl;
    std::cout << "RL: " << RL << std::endl;
    std::cout << "RR: " << RR << std::endl;
    std::cout << "km/t: " << p << std::endl;
    
    sleep(1);
    }
}
