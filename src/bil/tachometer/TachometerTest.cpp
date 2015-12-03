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

    while (1){
        
    FL = myDist.getDistance("FL");
    usleep(130000);
    FR = myDist.getDistance("FR");
    usleep(130000);
    RL = myDist.getDistance("RL");
    usleep(130000);
    RR = myDist.getDistance("RR");
    usleep(130000);
    p = myTacho.getVelocity();
    //usleep(90000);
    std::cout << "FL: " << FL << std::endl;
    std::cout << "FR: " << FR << std::endl;
    std::cout << "RL: " << RL << std::endl;
    std::cout << "RR: " << RR << std::endl;
    std::cout << "km/t: " << p << std::endl;
    //sleep(1);
 
    }
    return 0;
}
