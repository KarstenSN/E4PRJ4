#include "Tachometer.hpp"

int main() {
    
    Log myLog;
    
    Tachometer myTacho(&myLog);
    
    char p = 'X';
    while (1){
    p = myTacho.getVelocity();
    
    std::cout << "Received from I2C_bus: " << p << std::endl;
    
    sleep(1);
    }
}
