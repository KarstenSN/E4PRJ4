#include <iostream>
#include <string>
#include <wiringPi.h>
#include <pthread.h>
#include <thread>
#include <mutex>
#include <softPwm.h>
#include "Steering.hpp"
#include "Log.hpp"




// Pin number declarations. We're using the Broadcom chip pin numbers.
const int pwmMotorPin = 18; // PWM Motor - Broadcom pin 18, P1 pin 12
const int pwmServoPin = 23; // PWM Servo - Broadcom pin 19, P1 pin 33
const int pwmMotorForward = 6; // PWM Servo - Broadcom pin 6, P1 pin 31
const int pwmMotorBackward = 12; // PWM Servo - Broadcom pin 12, P1 pin 32
const int pwmSetClockValue = 2; // PWM Clock Set Value for hardware PWM between 2 to 4095
const int pwmSetRangeValue = 1024; //The default is 1024.
/*
pinMode(pwmMotor, PWM_OUTPUT); // Set pwmMotor as PWM output
pinMode(pwmMotorForward, OUTPUT); // Set pwmMotorForward as output
pinMode(pwmMotorBackward, OUTPUT); // Set pwmMotorBackward as output
*/
Steering::Steering(Data* dataClassPtr, Settings* MySettingsPtr, Log* MyLogPtr)
//: the_thread()
{
	dataClassPtr_ = dataClassPtr;
	settingsPtr_ = MySettingsPtr;
	logPtr_ = MyLogPtr;
	stop_thread = false;
  
	this->logPtr_->writeEvent(__PRETTY_FUNCTION__, "Steering constructor created");

	//***** Hardware PWM *****
	wiringPiSetupGpio(); // Initialize wiringPi -- using Broadcom pin numbers
	pinMode(pwmMotorPin, PWM_OUTPUT);// PWM_OUTPUT  /  OUTPUT
	//pwmSetClock (pwmSetClockValue) ; //This sets the divisor for the PWM clock
	pwmSetMode(PWM_MODE_MS); //default mode in the Pi is “balanced” PWM_MODE_BAL PWM_MODE_MS 
	pwmSetRange(pwmSetRangeValue); //The default is 1024.

	//PID 
	iGain_ = iGain; // integral gain
	pGain_ = pGain; // proportional gain
	dGain_ = dGain; // derivative gain
	iMax_ = iMax;
	iMin_ = iMin;
	minServoPWM_ = minServoPWM;
	maxServoPWM_ = maxServoPWM;

	//***** Software PWM *****
	err = softPwmCreate(pwmServoPin, 0, 200);
	if (err != 0)
	    this->logPtr_->writeError(__PRETTY_FUNCTION__, "Error creating softPwmCreate()");

	//std::thread motorPWMThread (&Steering::PWMUpdate, this );
	this->logPtr_->writeEvent(__PRETTY_FUNCTION__, "Constructor complete");
}

Steering::~Steering()
{
	softPwmWrite(pwmServoPin, 0);
	softPwmStop(pwmServoPin);
	stop_thread = true;
	if (motorPWMThread.joinable()) 
	  motorPWMThread.join();
	this->logPtr_->writeEvent(__PRETTY_FUNCTION__, "DeConstructor");
}

/**
* @param UsrInput_->forward  Den ønskede hastighed fremad  0..255. 255 er max af den indstillede hastighed
* @param UsrInput_->reverse Den ønskede hastighed bagud  0..255. 255 er max af den indstillede hastighed
* @param UsrInput_->turn Den ønskede dreje retning på forhjul. -127..127. -127 = fuldt udslag til venstre, center = 0, fuldt udslag til højre 127
* @param UsrInput_->stop Der skal bremses. Brems ikke = 0, Brems = 1.
*/
int Steering::userInput(UserInput* UsrInput_)
{	
	std::string msg;
	
	msg.append("userInput ").append(" forward: ").append(std::to_string(UsrInput_->forward))
	   .append(" reverse: ").append(std::to_string(UsrInput_->reverse))
	   .append(" turn: ").append(std::to_string(UsrInput_->turn))
	   .append(" stop: ").append(std::to_string(UsrInput_->stop));
	this->logPtr_->writeEvent(__PRETTY_FUNCTION__, msg);
	
	max_speed_ = (int)settingsPtr_->getMaxSpeed();
	
	if (UsrInput_->stop == 1)
		Steering::brake();
	else
	{
		if ((int)((max_speed_*UsrInput_->forward) / 255) != speedReqFor_ || (int)((max_speed_*UsrInput_->reverse) / 255) != speedReqBack_)
		{
			std::lock_guard<std::mutex> lock(changeVar_Mut);

			speedReqFor_ = (int)((max_speed_*UsrInput_->forward) / 255); 	//(maxSpeed/255)*forward adjust input with max speed 
			speedReqBack_ = (int)((max_speed_*UsrInput_->reverse) / 255);	//(maxSpeed/255)*reverse adjust input with max speed


			msg.append("userInput ").append(" speedReqFor_ ").append(std::to_string(speedReqFor_))
			   .append(" speedReqBack_ ").append(std::to_string(speedReqBack_));
			this->logPtr_->writeEvent(__PRETTY_FUNCTION__, msg);
			//std::cout << "userInput()" << "speedReqFor_ " << speedReqFor_ << " speedReqBack_ " << speedReqBack_ << std::endl;

		}

		Steering::motorSetPWM(UsrInput_->forward, UsrInput_->reverse);

		if ((int)UsrInput_->forward < 1 && (int)UsrInput_->reverse < 1)
		{
			Steering::softbrake();
		}

	}

	Steering::turn(UsrInput_->turn);

	return 1;
}

int Steering::brake()
{	
	this->logPtr_->writeEvent(__PRETTY_FUNCTION__, "brake");
	
	std::lock_guard<std::mutex> lock(changeVar_Mut);
	
	activatePWM_ = 0;
	pwmWrite(pwmMotorPin, 0);
	pinMode(pwmMotorPin, OUTPUT);
	digitalWrite(pwmMotorPin, HIGH);
	digitalWrite(pwmMotorForward, LOW);
	digitalWrite(pwmMotorBackward, LOW);
	return 1;

}

int Steering::softbrake()
{
	std::lock_guard<std::mutex> lock(changeVar_Mut);
	this->logPtr_->writeEvent(__PRETTY_FUNCTION__, "Softbrake activated");
	std::cout << "softbrake() called " << std::endl;
	activatePWM_ = 0;
	pwmWrite(pwmMotorPin, 0);
	digitalWrite(pwmMotorForward, LOW);
	digitalWrite(pwmMotorBackward, LOW);
	return 1;
}

int Steering::turn(int value)
{
	float TurnValue_ = ((((value + 127) * (maxServoPWM - minServoPWM)) / 255) + 1);
	softPwmWrite(pwmServoPin, TurnValue_);

	std::string msg;
	msg.append("Turn value received: ").append(std::to_string(value)).append(" Output value: ").append(std::to_string(TurnValue_));
	this->logPtr_->writeEvent(__PRETTY_FUNCTION__, msg);

	return 1;
}

int Steering::motorSetPWM(unsigned char speedForward, unsigned char speedBackward)
{
	this->logPtr_->writeEvent(__PRETTY_FUNCTION__, "MotorSetPWM entered");
	std::string msg;
	std::lock_guard<std::mutex> lock(changeVar_Mut);

	speedAct_ = dataClassPtr_->getLatestVelocity();

	if (speedForward >= 0)
	{
		if (speedAct_ > 5 && direction_ == 0)
		{
			Steering::brake();
			return 1;
		}

		pinMode(pwmMotorPin, PWM_OUTPUT);
		pwmSetMode(PWM_MODE_MS);
		pwmSetClock(pwmSetClockValue);
		digitalWrite(pwmMotorForward, HIGH);
		digitalWrite(pwmMotorBackward, LOW);
		direction_ = 1;
		activatePWM_ = 1;

		msg.append("SpeedForward >= 0: ").append(std::to_string(speedForward));
		this->logPtr_->writeEvent(__PRETTY_FUNCTION__, msg);
	}
	
	if (speedBackward > 0)
	{
		if (speedAct_ > 5 && direction_ == 1)
		{
			Steering::brake();
			return 1;
		}

		pinMode(pwmMotorPin, PWM_OUTPUT);
		pwmSetMode(PWM_MODE_MS);
		pwmSetClock(pwmSetClockValue);
		digitalWrite(pwmMotorForward, LOW);
		digitalWrite(pwmMotorBackward, HIGH);
		direction_ = 0;
		activatePWM_ = 1;
		std::string msg;
		msg.append("SpeedBackward > 0: ").append(std::to_string(speedForward));
		this->logPtr_->writeEvent(__PRETTY_FUNCTION__, msg);
	}

	return 1;

}

void Steering::PWMUpdate()
{
	std::string msg;

	this->logPtr_->writeEvent(__PRETTY_FUNCTION__, "thread starting");
	
	std::this_thread::sleep_for(std::chrono::microseconds(100));

	while (!stop_thread)
	{
// 		msg.append("PWMUpdate entry").append(" speedReqFor_: ").append(std::to_string(speedReqFor_))
// 		   .append(" speedAct_: ").append(std::to_string(speedAct_))
// 		   .append(" Direction: ").append(std::to_string(direction_));
// 		this->logPtr_->writeEvent(__PRETTY_FUNCTION__, msg);

		std::lock_guard<std::mutex> lock(changeVar_Mut);

		speedAct_ = dataClassPtr_->getLatestVelocity();

		// PID regulation
		if (direction_ == 1)
		{
			error_ = speedReqFor_ - speedAct_;
		}
		else
		{
			error_ = speedReqBack_ - speedAct_;
		}
		// calculate the proportional temp
		pTemp_ = pGain_ * error_;
		// calculate the integral state with appropriate limiting
		iState_ += error_;
		if (iState_ > iMax_)
		{
			iState_ = iMax_;
		}
		else if (iState_ < iMin_)
		{
			iState_ = iMin_;
		}
		iTemp_ = iGain_ * iState_; // calculate the integral temp
		dTemp_ = dGain_ * (dState_ - speedAct_); // calculate the  derivative temp
		dState_ = speedAct_;
		motorPWMOutValue = pTemp_ + dTemp_ + iTemp_;

		if (motorPWMOutValue <= 0)
		{
			motorPWMOutValue = 0;
		}

		if (motorPWMOutValue >= pwmSetRangeValue)
		{
			motorPWMOutValue = pwmSetRangeValue;
		}

		if (activatePWM_ == 1)
		{	
			//pinMode(pwmMotorPin, PWM_OUTPUT);
			pwmSetMode(PWM_MODE_MS);
			//pwmSetClock (pwmSetClockValue) ; //This sets the divisor for the PWM clock
			pwmWrite(pwmMotorPin, motorPWMOutValue);

		}

// 		std::cout << "updatePWM() called: PWM out :  " << motorPWMOutValue << " I " << iTemp_<< " D " << dTemp_ << " iState_ " << iState_<<   std::endl;

// 		msg.append("Before write ").append(std::to_string(motorPWMOutValue));
// 		this->logPtr_->writeEvent(__PRETTY_FUNCTION__, msg);

		std::this_thread::sleep_for(std::chrono::microseconds(5000)); // For testing
	}
	
	this->logPtr_->writeEvent(__PRETTY_FUNCTION__, "thread stopping");

}
