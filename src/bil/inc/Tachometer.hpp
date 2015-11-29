#pragma once

#include <iostream>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>

class Tachometer {
public:
  Tachometer(int slaveAddr){}
  ~Tachometer(){}
  char getVelocity(){ return 10;}
private:
  int tachoAddr;
};
