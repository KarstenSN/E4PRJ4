#pragma once

#include <iostream>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "Log.hpp"

class tachometer {
public:
  tachometer(Log *);
  ~tachometer();
  char getVelocity();
private:
  Log * myLogPtr;
};
