#include <Tachometer.hpp>

Tachometer::Tachometer(Log *log) {
    this->myLogPtr = log;
    myLogPtr->writeEvent(__PRETTY_FUNCTION__,"Created tachometer object.");
}

Tachometer::~Tachometer() {}

int Tachometer::getVelocity() {

    int file;
    int tachoAddr = 0x10;
    char buffer[9] = {0};
    // NB, denne skal sættes op individuelt på den PI der skal bruges...
     
    if ((file = open("/dev/i2c-1", O_RDWR)) < 0){
        //Error ***
        myLogPtr->writeError(__PRETTY_FUNCTION__,"Error opening I2C-bus from tachometer.");
        return -1;
         // I2C bus opening error
    }

    if ((ioctl(file, I2C_SLAVE, tachoAddr)) < 0){
        //Error
        myLogPtr->writeError(__PRETTY_FUNCTION__,"Communication error via I2C to tachometer.");
        return -2; 
        // Connecting failure
    }

    //Reading I2C tachometer
    int check = read(file,buffer,9);
    if ((check) != 9){
        //Error
        myLogPtr->writeError(__PRETTY_FUNCTION__,"Failed to read tachometer from I2C bus.");
        close (file);
        return -3;    
         // Read failure
    }
    else if (buffer[8] >= 0 && buffer[8] < 254) {
        close (file);
        return ((int)buffer[8]);
    }
    else {
        close (file);
        myLogPtr->writeWarning(__PRETTY_FUNCTION__,"Tachometer received value that was out of bounds from PSoC.");
        close (file);
        return -4;  
        // Out of bounds
    }
    
}

