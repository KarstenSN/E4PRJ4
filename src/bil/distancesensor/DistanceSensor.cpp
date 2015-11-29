//DistanceSensor.cpp

#include "DistanceSensor.hpp"
#include <linux/i2c-dev.h>
#include "Log.hpp"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int adapter_nr = 1;                             // Valg af I2C-bus
char filename[20];
int fd= 0;

// Constructor
DistanceSensor::DistanceSensor(){

// Initialisering af adresser
    this->addrFL = 0b01110000;                  // 0x70 dec 112
    this->addrFR = 0b01110001;                  // 0x71 dec 113
    this->addrRL = 0b01110011;                  // 0x73 dec 115
    this->addrRR = 0b01110110;                  // 0x76 dec 118
    
// Initialisering af afstande
    this->distanceFL = 0;
    this->distanceFR = 0;
    this->distanceRL = 0;
    this->distanceRR = 0;    
    
// open i2c device;
    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);

    std::cout << "opening: "<< filename << std::endl;

    if (fd = open(filename, O_RDWR) < 0){
        std::cout << "Error in opening: %s" << filename << std::endl;
        exit(1);
    }

// Sæt adresse på sensor:
    // FrontLeft
    if (ioctl(fd, I2C_SLAVE, addrFL) < 0){
        std::cout << "Error in setting addr: %d" << addrFL << std::endl;
        exit(1);
    }
    // FrontRight
    if (ioctl(fd, I2C_SLAVE, addrFR) < 0){
        std::cout << "Error in setting addr: %d" << addrFR << std::endl;
        exit(1);
    }
    // RearLeft
    if (ioctl(fd, I2C_SLAVE, addrRL) < 0){
        std::cout << "Error in setting addr: %d" << addrRL << std::endl;
        exit(1);
    }
    // RearRight
    if (ioctl(fd, I2C_SLAVE, addrRR) < 0){
        std::cout << "Error in setting addr: %d" << addrRR << std::endl;
        exit(1);
    }
}

// Close Device()
DistanceSensor::~DistanceSensor(){
    close(fd);
}


// getDistance()
char DistanceSensor::getDistance(std::string name){

    if(name == "FL"){
        return distanceFL;
    }
    if(name == "FR"){
        return distanceFR;
    }
    if(name == "RL"){
        return distanceRL;
    }
    if(name == "RR"){
        return distanceRR;
    }
return 0;
}


// returnDistance()
void DistanceSensor::returnDistance(){

// MaxSonar1202 commands:
    int FLwrite[1] = { (this->addrFL << 1) & 11111110};     // adressen sættes til write-enable
    int FLread[1]  = { (this->addrFL << 1) + 1};            // adressen sættes til read-enable
    int FRwrite[1] = { (this->addrFR << 1) & 11111110};
    int FRread[1]  = { (this->addrFR << 1) + 1};
    int RLwrite[1] = { (this->addrRL << 1) & 11111110};
    int RLread[1]  = { (this->addrRL << 1) + 1};
    int RRwrite[1] = { (this->addrRR << 1) & 11111110};
    int RRread[1]  = { (this->addrRR << 1) + 1};
    
// Write "Start Range-reeading ved alle sensorer:
    //FrontLeft
    if (write(fd, FLwrite, 1) != 1){
        std::cout << "Error in start Range-reading at addr: %d" << addrFL << std::endl;
    }
    // Front Right
    if (write(fd, FRwrite, 1) != 1){
        std::cout << "Error in start Range-reading at addr: %d" << addrFR << std::endl;
    }
    // Rear Left
    if (write(fd, RLwrite, 1) != 1){
        std::cout << "Error in start Range-reading at addr: %d" << addrRL << std::endl;
    }
    // Rear Right
    if (write(fd, RRwrite, 1) != 1){
        std::cout << "Error in start Range-reading at addr: %d" << addrRR << std::endl;
    }

//Læse seneste Range fra alle 4 sensorer
    // Front Left
    int dataFL[2];
    if (write(fd, FLread, 1) != 1){
        std::cout << "Error in Range-reading at addr: %d" << addrFL << std::endl;
    }
    if (read(fd, dataFL, 2) != 2) {
        std::cout << "Error in get Range-reading at addr: %d" << addrFL << std::endl;
    }
    distanceFL = (dataFL[0] << 4) + dataFL[1];

    // Front Right
    int dataFR[2];
    if (write(fd, FRread, 1) != 1){
        std::cout << "Error in Range-reading at addr: %d" << addrFR << std::endl;
    }
    if (read(fd, dataFR, 2) != 2) {
        std::cout << "Error in get Range-reading at addr: %d" << addrFR << std::endl;
    }
    distanceFR = (dataFR[0] << 4) + dataFR[1];

    // Rear left
    int dataRL[2];
    if (write(fd, RLread, 1) != 1){
        std::cout << "Error in Range-reading at addr: %d" << addrRL << std::endl;
    }
    if (read(fd, dataRL, 2) != 2) {
        std::cout << "Error in get Range-reading at addr: %d" << addrRL << std::endl;
    }
    distanceRL = (dataRL[0] << 4) + dataRL[1];

    // Rear Right
    int dataRR[2];
    if (write(fd, RRread, 1) != 1){
        std::cout << "Error in Range-reading at addr: %d" << addrRR << std::endl;
    }
    if (read(fd, dataRR, 2) != 2) {
        std::cout << "Error in get Range-reading at addr: %d" << addrRR << std::endl;
    }
    distanceRR = (dataRR[0] << 4) + dataRR[1];
}


    // skriv i Log


/* Syntax til Log-klassen
*
* Log->writeEvent(from (string), msg (string))
*
* Event / Error / Warning
*/
