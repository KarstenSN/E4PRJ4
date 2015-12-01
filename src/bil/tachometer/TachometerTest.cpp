#include <string.h>
#include "Tachometer.hpp"
#include "DistanceSensor.hpp"

int main() {
    
    Log myLog;
    
    Tachometer myTacho(&myLog);
    DistanceSensor myDist(&myLog);
    
    int p = 0;
    int FL = 0;
    int FR = 0;
    int RL = 0;
    int RR = 0;
    std::string FLstr= "FL";
    while (1){
        
    FL = myDist.getDistance(FLstr);
    //sleep(10);
    FR = myDist.getDistance("FR");
    //sleep(10);
    RL = myDist.getDistance("RL");
    //sleep(10);
    RR = myDist.getDistance("RR");
    //sleep(10);
    p = myTacho.getVelocity();
    //sleep(10);
    std::cout << "FL: " << FL << std::endl;
    std::cout << "FR: " << FR << std::endl;
    std::cout << "RL: " << RL << std::endl;
    std::cout << "RR: " << RR << std::endl;
    std::cout << "km/t: " << p << std::endl;
    
    sleep(1);
    }
    return 0;
}
