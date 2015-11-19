#include "tachometer.hpp"

tachometer::tachometer(int slaveAddr) {
    tachometer::tachoAddr = slaveAddr;
}

tachometer::~tachometer() {}

char tachometer::getVelocity() {

    int file;
    int tachoAddr = 0x08;
    char buffer[2];
    // NB, denne skal sættes op individuelt på den PI der skal bruges...
     
    if ((file = open("/dev/bcm2708_i2c.1", O_RDWR)) < 0){
        //Error ***
        std::cout << "Error opening I2C-bus from tachometer.\n" << std::endl;
        return 'I'; // I2C bus opening error
    }

    if ((ioctl(file, I2C_SLAVE, tachoAddr)) < 0){
        //Error
        std::cout << "Error connecting and/or communicating via I2C to tachometer.\n" << std::endl;
        return 'C'; // Connecting failure
    }

    //Reading I2C tachometer
    if ((read(file,buffer,1)) != 1){
        //Error
        std::cout << "Failed to read tachometer from I2C bus.\n" << std::endl;
        return 'R';     // Read failure
    }
    else if (buffer[0] > 0 && buffer[0] < 254) {
        return buffer[0];
    }
    else {
        return 'O';  // Out of bounds
    }
    
}

