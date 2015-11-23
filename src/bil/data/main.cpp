#include <iostream>
#include "data.hpp"

int main(){
    
    std::cout << "Main running!" << std::endl;
    
    Data d;
    
    d.writeVelocity(10);
    d.writeAcceleration(4);
    d.writeDistance("FR", 45);
    
    std::cout << "Velocity: " << d.getLatestVelocity() << std::endl;
    std::cout << "Acceleration: " << d.getLatestAcceleration() << std::endl;
    std::cout << "Distance FR: " << d.getLatestDistance("FR") << std:: endl;
    
    return 0;
}