#include "tachometer.hpp"

int main() {
    
    int myTachoAddr = 0x08;
    
    tachometer myTacho(myTachoAddr);
    
    char p = 'X';
    while (1){
    p = myTacho.getVelocity();
    
    std::cout << "Received from I2C_bus: " << p << std::endl;
    
    sleep(1);
    }
}
