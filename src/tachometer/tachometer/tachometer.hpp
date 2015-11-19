#pragma once

#include <iostream>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>

class tachometer {
public:
  tachometer(int slaveAddr);
  ~tachometer();
  char getVelocity();
private:
  int tachoAddr;
};
