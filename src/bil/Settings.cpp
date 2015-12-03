#include <Settings.hpp>

Settings::Settings(Log* logClassPtr)
{
  this->logClassPtr_ = logClassPtr;
  this->maxVelocity_ = 0;
  this->AKSStatus_ = 0;
  this->calibration_ = 0;
  this->logClassPtr_->writeEvent(__PRETTY_FUNCTION__,"Initialized Settings");
}

void Settings::setMaxSpeed(int maxVelocity)
{
  std::lock_guard<std::mutex> lock(this->maxVelocityMut);
  this->maxVelocity_ = maxVelocity;
  return;
}

void Settings::setAKS(bool AKSStatus)
{
  std::lock_guard<std::mutex> lock(this->AKSStatusMut);
  this->AKSStatus_ = AKSStatus;
  return;
}

void Settings::calibrateSteering(char calibration)
{
  std::lock_guard<std::mutex> lock(this->calibrationMut);
  this->calibration_ = calibration;
  return;
}

int Settings::getMaxSpeed()
{
  std::lock_guard<std::mutex> lock(this->maxVelocityMut);
  return this->maxVelocity_;
}

bool Settings::getAKS()
{
  std::lock_guard<std::mutex> lock(this->AKSStatusMut);
  return this->AKSStatus_;
}

char Settings::getCalibration()
{
  std::lock_guard<std::mutex> lock(this->calibrationMut);
  return this->calibration_;
}