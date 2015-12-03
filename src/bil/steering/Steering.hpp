﻿#pragma once
#include "Data.hpp"
#include "utilities.hpp"
#include "Settings.hpp"
#include "Log.hpp"
#include <thread>
#include <string>
#include <mutex>
#include <chrono>

#define	pGain	2
#define	iGain	0.1
#define	dGain	0.1
#define	iMax	10000
#define	iMin	0
#define minServoPWM 5 // 0,5 ms
#define	maxServoPWM 25 // 2,5 ms


	
class Steering
{
public:
	Steering(Data* dataClassPtr, Settings* MySettingsPtr, Log* MyLogPtr);
	~Steering();
	int userInput(UserInput* UsrInput_);
	void PWMUpdate();
	bool stop_thread;
	
	
private:
	void getPWMvar(int &PWMforward, int &PWMBackward, bool &PWMdirection);
	int brake();
	int softbrake();
	int turn(int value);
	int motorSetPWM(unsigned char speedForward, unsigned char speedBackward );
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
	
	std::mutex changeVar_Mut;
	
	int err = 0;
	
	
};