#include <iostream>
#include "steering.hpp"
//#include "wiringPi.h"
//#include <wiringPi.h>
#include "data.hpp"

// Pin number declarations. We're using the Broadcom chip pin numbers.
const int pwmMotor = 18; // PWM Motor - Broadcom pin 18, P1 pin 12
const int pwmServo = 24; // PWM Servo - Broadcom pin 19, P1 pin 33
const int pwmMotorForward = 6; // PWM Servo - Broadcom pin 6, P1 pin 31
const int pwmMotorBackward = 12; // PWM Servo - Broadcom pin 12, P1 pin 32
/*
pinMode(pwmMotor, PWM_OUTPUT); // Set pwmMotor as PWM output
pinMode(pwmServo, PWM_OUTPUT); // Set pwmServo as PWM output
pinMode(pwmMotorForward, OUTPUT); // Set pwmMotorForward as output
pinMode(pwmMotorBackward, OUTPUT); // Set pwmMotorBackward as output
*/
steering::steering(Data* dataClassPtr)
{
	std::cout << "constructor" << std::endl;
	dataClassPtr_ = dataClassPtr;
/*	direction_ = 1;
	pwm_ = 0;
	wiringPiSetupGpio(); // Initialize wiringPi -- using Broadcom pin numbers
		
	//	//pwmSetClock (int divisor) ; //This sets the divisor for the PWM clock
	pwmSetMode (PWM_MODE_MS) ; //default mode in the Pi is “balanced”
	pwmSetRange (2500) ; //The default is 1024.
	iGain_ = iGain; // integral gain
	pGain_ = pGain; // proportional gain
	dGain_ = dGain; // derivative gain
	iMax_ = iMax;
	iMin_ = iMin;
	minServoPWM_ = minServoPWM;
	maxServoPWM_ = maxServoPWM;
*/	
	
}


steering::~steering()
{

	
}



/**
* @param speedForward  Den ønskede hastighed fremad  0..255. 255 er max af den indstillede hastighed
* @param speedBackward Den ønskede hastighed bagud  0..255. 255 er max af den indstillede hastighed
* @param turn Den ønskede dreje retning på forhjul. -127..127. -127 = fuldt udslag til venstre, center = 0, fuldt udslag til højre 127
* @param brake Der skal bremses. Brems ikke = 0, Brems = 1.
*/
int steering::userInput(unsigned char speedForward, unsigned char speedBackward, char turn, char brake)
{
	speedAct_ = dataClassPtr_->getLatestVelocity();
	
	std::cout << "userInput spdFor: " << speedForward << " spdBack: " << speedBackward << " turn: " << turn << " brake: " << brake <<   std::endl;
	if(brake==1)
		steering::brake();
	else
	{
		if(	speedForward != speedReqFor_ || speedBackward != speedReqBack_)
		{
			speedReqFor_ = speedForward;
			speedReqBack_ = speedBackward;
			//steering::motorSetPWM( speedForward, speedBackward);
		}		
	}
	
	steering::turn(turn);
	
	return 1;
}

int steering::brake()
{
	std::cout << "Brake() called " << std::endl;
	/*pinMode(pwmMotor, OUTPUT);
	digitalWrite(pwmMotor, HIGH) ;
	digitalWrite(pwmMotorForward, LOW) ;
	digitalWrite(pwmMotorBackward, LOW) ;*/
	pwm_ = 0;
	return 1;

}

int steering::softbrake()
{/*
	digitalWrite (pwmMotor, LOW) ;
	digitalWrite (pwmMotorForward, LOW) ;
	digitalWrite (pwmMotorBackward, LOW) ;*/
	std::cout << "softbrake() called " << std::endl;
	pwm_ = 0;
	return 1;
}


int steering::turn(int value)
{/*
	pinMode(pwmServo, PWM_OUTPUT);
	float value_ = (value + 127)/(255/(maxServoPWM-minServoPWM));
	int servoPWM = (int)(minServoPWM + value_);
	
	pwmWrite(pwmServo, servoPWM) ;*/
	std::cout << "turn() called:  " << value << std::endl;
	return 1;
}

int steering::motorSetPWM(unsigned char speedForward, unsigned char speedBackward)
{
	
	if(speedForward > 2 && direction_ == 0)
		{
			steering::brake();
			pwm_ = 0;
			return 1;
		}
	if(speedBackward > 2 && direction_ == 1)
		{
			steering::brake();
			pwm_ = 0;
			return 1;
		}
	
//	pinMode(pwmMotor, PWM_OUTPUT);
	
	if(speedForward > 0)
		{
//			digitalWrite(pwmMotorForward, HIGH);
//			digitalWrite(pwmMotorBackward, LOW);
			std::cout << "motorSetPWM() called: speedForward:  " << speedForward << std::endl;
			direction_ = 1;
			steering::updatePWM();
		}
	else
		{
//			digitalWrite(pwmMotorForward, LOW);
//			digitalWrite(pwmMotorBackward, HIGH); 
			std::cout << "motorSetPWM() called: speedbackward:  " << speedBackward << std::endl;
			direction_ = 0;
			steering::updatePWM();
		}
	

	return 1;
}

int steering::updatePWM()
{
	
	if(direction_ == 1)
		{error_ = speedReqFor_ - speedAct_;}
	else
	{	error_ = speedReqBack_ - speedAct_;}
	
	pTemp_ = pGain_ * error_; // calculate the proportional temp
	// calculate the integral state with appropriate limiting
	iState_ += error_;
	if (iState_ > iMax_) iState_ = iMax_;
	else if (iState_ < iMin_) iState_ = iMin_;
	iTemp_ = iGain_ * iState_; // calculate the integral temp
	dTemp_ = dGain_ * (dState_ - speedAct_); // calculate the  derivative temp
	dState_ = speedAct_;
	pwm_ = pTemp_ + dTemp_ + iTemp_;
	std::cout << "updatePWM() called: PWM out :  " << pwm_ << std::endl;
//	pwmWrite(pwmMotor, pwm_) ;

return 1;
}	
