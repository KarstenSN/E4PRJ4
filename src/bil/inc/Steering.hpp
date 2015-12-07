#pragma once
#include <pthread.h>
#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <chrono>
#include <stdlib.h>
#include <softPwm.h>
#include <wiringPi.h>
#include <Data.hpp>
#include <utilities.hpp>
#include <Settings.hpp>
#include <Log.hpp>

#define	pGain	20
#define	iGain	0.1
#define	dGain	0.1
#define	iMax	2300
#define	iMin	0
#define minServoPWM 5 // 0,5 ms
#define	maxServoPWM 25 // 2,5 ms

#define PWM_CLOCK_FREQ 19200000
#define PWM_MOTOR_PIN  18 // PWM Motor - Broadcom pin 18, P1 pin 12
#define PWM_SERVO_PIN  23 // PWM Servo - Broadcom pin 19, P1 pin 33
#define PWM_MOTOR_FORWARD  6 // PWM Servo - Broadcom pin 6, P1 pin 31
#define PWM_MOTOR_BACKWARD  12 // PWM Servo - Broadcom pin 12, P1 pin 32
#define PWM_SET_CLOCK_VALUE  2 // PWM Clock Set Value for hardware PWM between 2 to 4095
#define PWM_SET_RANGE_VALUE  240 //The default is 1024.
#define PWM_FREQ 40000 

class Steering
{
public:
	Steering(Data* dataClassPtr, Settings* MySettingsPtr, Log* MyLogPtr);
	~Steering();
	int userInput(UserInput* UsrInput_);
	//void PWMUpdate();
	


private:
	void PWMUpdate();
	void getPWMvar(int &PWMforward, int &PWMBackward, bool &PWMdirection);
	int brake();
	int softbrake();
	int turn(signed char value);
	int motorSetPWM(unsigned char speedForward, unsigned char speedBackward);
	int speedReqFor_ = 0;
	int speedReqBack_ = 0;
	int speedAct_ = 0;
	int max_speed_ = 0;
	int motorPWMOutValue = 0;
	bool activatePWM_ = 0;
	bool direction_ = 1; // 1 = forward, 0 = Backward
	int minServoPWM_;
	int maxServoPWM_;
	Data* dataClassPtr_;
	Settings* settingsPtr_;
	Log* logPtr_;

	double dState_ = 0; // Last position input
	double iState_ = 0; // Integrator state
	double iMax_ = 0, iMin_ = 0; // Maximum and minimum allowable integrator state
	double iGain_ = 0; // integral gain
	double pGain_ = 0; // proportional gain
	double dGain_ = 0; // derivative gain
	double error_ = 0;
	double pTemp_ = 0, dTemp_ = 0, iTemp_ = 0;
	std::thread motorPWMThread;
	bool stop_thread;
	std::mutex changeVar_Mut;

	int err = 0;


};