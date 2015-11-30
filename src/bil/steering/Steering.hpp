#pragma once
#include "Data.hpp"
#include "utilities.hpp"
#include "Settings.hpp"
#include "Log.hpp"
#include <thread>
#include <string>
#include <mutex>

#define	pGain	1
#define	iGain	0.0001
#define	dGain	0.0001
#define	iMax	5
#define	iMin	50
#define minServoPWM 5 // 0,5 ms
#define	maxServoPWM 69 // 2,5 ms


	
class Steering
{
public:
	Steering(Data* dataClassPtr, Settings* MySettingsPtr, Log* MyLogPtr);
	~Steering();
	int userInput(UserInput* UsrInput_);
	void PWMUpdate();
	
	
	
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
	int pwm_ = 0;
	bool activatePWM_ ;
	bool direction_; // 1 = forward, 0 = Backward
	int minServoPWM_;
	int maxServoPWM_;
	Data* dataClassPtr_;
	Settings* settingsPtr_;
	Log* logPtr_;
	
	double dState_; // Last position input
	double iState_; // Integrator state
	double iMax_, iMin_; // Maximum and minimum allowable integrator state
	double iGain_; // integral gain
	double pGain_; // proportional gain
	double dGain_; // derivative gain
	double error_;
	double pTemp_, dTemp_, iTemp_;
	//std::thread the_thread;
	bool stop_thread;
	std::mutex getPWMvar_Mut;
	
	
	
	
};

// void SteeringStartpwm(Steering*  car, Log* log);

/* JUNK

bool goForward(int speed);
bool goBackward(int speed);
void awaitNewInput();
void analyseInput();
bool blockUser();
bool turnLeft();
bool turnRight();

*/