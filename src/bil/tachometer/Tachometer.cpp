#include "Tachometer.hpp"

Tachometer::Tachometer(Log *log) {
    this->myLogPtr = log;
}

Tachometer::~Tachometer() {}

char Tachometer::getVelocity() {

    int file;
    int tachoAddr = 0x10;
    char buffer[9];
    // NB, denne skal sættes op individuelt på den PI der skal bruges...
     
    if ((file = open("/dev/bcm2708_i2c.1", O_RDWR)) < 0){
        //Error ***
        myLogPtr->writeError(__PRETTY_FUNCTION__,"Error opening I2C-bus from tachometer.\n");
        return 'I';
         // I2C bus opening error
    }

    if ((ioctl(file, I2C_SLAVE, tachoAddr)) < 0){
        //Error
        myLogPtr->writeError(__PRETTY_FUNCTION__,"Communication error via I2C to tachometer.\n");
        return 'C'; 
        // Connecting failure
    }

    //Reading I2C tachometer
    if ((read(file,buffer,9)) != 9){
        //Error
        myLogPtr->writeError(__PRETTY_FUNCTION__,"Failed to read tachometer from I2C bus.\n");
        return 'R';    
         // Read failure
    }
    else if (buffer[8] > 0 && buffer[8] < 254) {
        return buffer[8];
    }
    else {
        return 'O';  
        // Out of bounds
    }
    
}

