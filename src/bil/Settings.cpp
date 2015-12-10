#include <Settings.hpp>
//----------Settings::Settings1----------
Settings::Settings(Log* logClassPtr)
{
  this->logClassPtr_ = logClassPtr;
  this->maxVelocity_ = 0;
  this->AKSStatus_ = 0;
  this->calibration_ = 0;
  this->logClassPtr_->writeEvent(__PRETTY_FUNCTION__,"Initialized Settings");
}
//----------Settings::Settings2----------

//----------Settings::setMaxSpeed1----------
void Settings::setMaxSpeed(int maxVelocity)
{
  std::lock_guard<std::mutex> lock(this->maxVelocityMut);
  this->maxVelocity_ = maxVelocity;
  return;
}
//----------Settings::setMaxSpeed2----------

//----------Settings::setAKS1----------
void Settings::setAKS(bool AKSStatus)
{
  std::lock_guard<std::mutex> lock(this->AKSStatusMut);
  this->AKSStatus_ = AKSStatus;
  return;
}
//----------Settings::setAKS2----------

//----------Settings::calibrateSteering1----------
void Settings::calibrateSteering(char calibration)
{
  std::lock_guard<std::mutex> lock(this->calibrationMut);
  this->calibration_ = calibration;
  return;
}
//----------Settings::calibrateSteering2----------

//----------Settings::getMaxSpeed1----------
int Settings::getMaxSpeed()
{
  std::lock_guard<std::mutex> lock(this->maxVelocityMut);
  return this->maxVelocity_;
}
//----------Settings::getMaxSpeed2----------

//----------Settings::getAKS1----------
bool Settings::getAKS()
{
  std::lock_guard<std::mutex> lock(this->AKSStatusMut);
  return this->AKSStatus_;
}
//----------Settings::getAKS2----------

//----------Settings::getCalibration1----------
char Settings::getCalibration()
{
  std::lock_guard<std::mutex> lock(this->calibrationMut);
  return this->calibration_;
}
//----------Settings::getCalibration2----------