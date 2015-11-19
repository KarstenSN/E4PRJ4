#pragma once

#include <iostream>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "Log.hpp"

class Tachometer {
public:
  Tachometer(Log *);
  ~Tachometer();
  char getVelocity();
private:
  Log * myLogPtr;
};
