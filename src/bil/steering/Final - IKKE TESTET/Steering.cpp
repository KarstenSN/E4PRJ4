#include <iostream>
#include "Steering.hpp"
#include <wiringPi.h>
#include <pthread.h>
#include <thread>
#include "Log.hpp"
#include <string>
#include <mutex>


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
Steering::Steering(Data* dataClassPtr, Settings* MySettingsPtr, Log* MyLogPtr): the_thread()
{
	std::cout << "Steering constructor" << std::endl;
	dataClassPtr_ = dataClassPtr;
	settingsPtr_= MySettingsPtr;
	logPtr_ = MyLogPtr;
	direction_ = 1;
	pwm_ = 0;
	stop_thread = false;	

	wiringPiSetupGpio(); // Initialize wiringPi -- using Broadcom pin numbers
	      //pwmSetClock (int divisor) ; //This sets the divisor for the PWM clock
	pwmSetMode (PWM_MODE_MS) ; //default mode in the Pi is “balanced”
	
	pwmSetRange (2500) ; //The default is 1024.
	iGain_ = iGain; // integral gain
	pGain_ = pGain; // proportional gain
	dGain_ = dGain; // derivative gain
	iMax_ = iMax;
	iMin_ = iMin;
	minServoPWM_ = minServoPWM;
	maxServoPWM_ = maxServoPWM;
	logPtr_->writeEvent(__PRETTY_FUNCTION__, "Constructor");
}

Steering::~Steering()
{
	stop_thread = true;
        if(the_thread.joinable()) the_thread.join();
	logPtr_->writeEvent(__PRETTY_FUNCTION__, "DEConstructor");
}



/**
* @param UsrInput_->forward  Den ønskede hastighed fremad  0..255. 255 er max af den indstillede hastighed
* @param UsrInput_->reverse Den ønskede hastighed bagud  0..255. 255 er max af den indstillede hastighed
* @param UsrInput_->turn Den ønskede dreje retning på forhjul. -127..127. -127 = fuldt udslag til venstre, center = 0, fuldt udslag til højre 127
* @param UsrInput_->stop Der skal bremses. Brems ikke = 0, Brems = 1.
*/
int Steering::userInput(UserInput* UsrInput_)
{
	logPtr_->writeEvent(__PRETTY_FUNCTION__, "input");
	std::cout << "userInput spdFor: " << (int)UsrInput_->forward << " spdBack: " << (int)UsrInput_->reverse << " turn: " << (int)UsrInput_->turn << " brake: " << (int)UsrInput_->stop <<   std::endl;	
// 	max_speed_ = (int)settingsPtr_->getMaxSpeed();
	max_speed_ = 5;
	if(UsrInput_->stop==1)
	      Steering::brake();
	else
	{
		if((float)((max_speed_/255)*UsrInput_->forward) != speedReqFor_ || (float)((max_speed_/255)*UsrInput_->reverse) != speedReqBack_)
		{
		      std::lock_guard<std::mutex> lock(getPWMvar_Mut);
		      logPtr_->writeEvent(__PRETTY_FUNCTION__, "input new");
		      speedReqFor_ = (int)((max_speed_*UsrInput_->forward) / 255); 	//(maxSpeed/255)*forward adjust input with max speed 
		      speedReqBack_ = (int)((max_speed_*UsrInput_->reverse) / 255);	//(maxSpeed/255)*reverse adjust input with max speed
		      std::cout << "userInput()"  << "speedReqFor_ " << speedReqFor_ << " speedReqBack_ " << speedReqBack_<<  std::endl; 
		      std::string msg;
		      msg.append("PWMUpdate entry").append(" speedReqFor_ ").append(std::to_string(speedReqFor_)).append(" speedReqBack_ ").append(std::to_string(speedReqBack_))  ;
		      logPtr_->writeEvent(__PRETTY_FUNCTION__, msg );
		      
		      
		}
		
		Steering::motorSetPWM( UsrInput_->forward, UsrInput_->reverse );
		
		if((int)UsrInput_->forward < 1 && (int)UsrInput_->reverse < 1)
		{
		    Steering::softbrake();
		}
		
	}
	
	Steering::turn(UsrInput_->turn);
	
	return 1;
}

int Steering::brake()
{
	logPtr_->writeEvent(__PRETTY_FUNCTION__, "brake");
	std::cout << "Brake() called " << std::endl;
	activatePWM_ = 0;
	pwmWrite(pwmMotor, 0) ;
	pinMode(pwmMotor, OUTPUT);
	digitalWrite(pwmMotor, HIGH) ;
	digitalWrite(pwmMotorForward, LOW) ;
	digitalWrite(pwmMotorBackward, LOW) ;
	return 1;

}

int Steering::softbrake()
{
	logPtr_->writeEvent(__PRETTY_FUNCTION__, "softbrake");
	std::cout << "softbrake() called " << std::endl;
	activatePWM_ = 0;
	pwmWrite(pwmMotor, 0) ;
	digitalWrite (pwmMotor, LOW) ;
	digitalWrite (pwmMotorForward, LOW) ;
	digitalWrite (pwmMotorBackward, LOW) ;
	return 1;
}


int Steering::turn(int value)
{	
	logPtr_->writeEvent(__PRETTY_FUNCTION__, "turn1");
	std::cout << "turn() called:  " << value << std::endl;
	pinMode(pwmServo, PWM_OUTPUT);
	logPtr_->writeEvent(__PRETTY_FUNCTION__, "turn2");
	float value_ = (value + 127)/(255/(maxServoPWM-minServoPWM));
	logPtr_->writeEvent(__PRETTY_FUNCTION__, "turn3");
	int servoPWM = (int)(minServoPWM + value_);
	logPtr_->writeEvent(__PRETTY_FUNCTION__, "turn4");
	pwmWrite(pwmServo, servoPWM) ;
	logPtr_->writeEvent(__PRETTY_FUNCTION__, "turn5");
	return 1;
}

int Steering::motorSetPWM(unsigned char speedForward, unsigned char speedBackward)
{
	logPtr_->writeEvent(__PRETTY_FUNCTION__, "motorSetPWM");
	if((int)speedForward > 2 && direction_ == 0)
		{
			Steering::brake();
			return 1;
		}
	if((int)speedBackward > 2 && direction_ == 1)
		{
			Steering::brake();
			return 1;
		}
	
	pinMode(pwmMotor, PWM_OUTPUT);
	
	std::lock_guard<std::mutex> lock(getPWMvar_Mut);
	if((int)speedForward > 0)
		{
			logPtr_->writeEvent(__PRETTY_FUNCTION__, "speedForward > 0");
			std::cout << "motorSetPWM() called: speedForward:  " << (int)speedForward << std::endl;
			digitalWrite(pwmMotorForward, HIGH);
			digitalWrite(pwmMotorBackward, LOW);
			direction_ = 1;
			activatePWM_ = 1;
			//Steering::updatePWM();
		}
	else
		{
			logPtr_->writeEvent(__PRETTY_FUNCTION__, "speedbackward > 0");
			std::cout << "motorSetPWM() called: speedbackward:  " << speedBackward << std::endl;
			digitalWrite(pwmMotorForward, LOW);
			digitalWrite(pwmMotorBackward, HIGH); 
			direction_ = 0;
			activatePWM_ = 1;
			//Steering::updatePWM();
		}
	
return 1;
	
}

//void *Steering::PWMUpdate(void* )
void Steering::PWMUpdate()
{	
  bool testwhile = true;
  int whilecount =0 ;
  
//   int PWMforward = 0;
//   int PWMBackward = 0;
//   bool PWMdirection = 0;
//   
//   
// 	Steering::getPWMvar( PWMforward, PWMBackward,  PWMdirection);
//   
 // while(testwhile==){}
  
//	std::string msg;
//	msg.append("PWMUpdate entry").append(" speedReqFor_ ").append(std::to_string(PWMforward)).append(" speedAct_ ").append(std::to_string(speedAct_))  ;
//	logPtr_->writeEvent(__PRETTY_FUNCTION__, msg );
 	std::cout << "updatePWM() called: PWM "  << std::endl;
	while(testwhile)
// 	while(!stop_thread)
	{
	  
// 	Steering::getPWMvar( PWMforward, PWMBackward,  PWMdirection);
	  std::lock_guard<std::mutex> lock(getPWMvar_Mut);
	  
	  logPtr_->writeEvent(__PRETTY_FUNCTION__, "while");

	  speedAct_ = 2;
	  std::cout << "PWMUpdate()"  << "speedReqFor_ " << (int)speedReqFor_ << " speedAct_ " << (int)speedAct_<<  std::endl; 
	  
	    if(direction_ == 1)
		    {error_ = speedReqFor_ - speedAct_;}
	    else
	    {	error_ = speedReqBack_ - speedAct_;}
	    // calculate the proportional temp
	    pTemp_ = pGain_ * error_; 
	    // calculate the integral state with appropriate limiting
	    iState_ += error_;
	    if (iState_ > iMax_) iState_ = iMax_;
	    else if (iState_ < iMin_) iState_ = iMin_;
	    iTemp_ = iGain_ * iState_; // calculate the integral temp
	    dTemp_ = dGain_ * (dState_ - speedAct_); // calculate the  derivative temp
	    dState_ = speedAct_;
	    pwm_ = pTemp_ + dTemp_ + iTemp_;
	    std::cout << "updatePWM() called: PWM out before:  " << pwm_  << std::endl;
	    
	    if(pwm_ <= 0)
	    {
		pwm_=0;
	    }
	    
	     if(pwm_ >= 800)
	    {
		pwm_= 800;
	    }
	     
	    if(activatePWM_ == 1)
	    {
	      pwmWrite(pwmMotor, pwm_) ;
	    }
	    std::cout << "updatePWM() called: PWM out :  " << pwm_ << " count " << whilecount << std::endl;
	    
	    whilecount++;
	if(whilecount >50)
	  testwhile=0;
	//std::this_thread::sleep_for(1);
	}
	//std::string msg = "PID:" + "P: " + pTemp_ ;
	logPtr_->writeEvent(__PRETTY_FUNCTION__, "thread stopping"  );
	

	
	
}	


void Steering::getPWMvar(int &PWMforward, int &PWMBackward, bool &PWMdirection)
{
  std::lock_guard<std::mutex> lock(getPWMvar_Mut);
  PWMforward = speedReqFor_;
  PWMBackward = speedReqBack_;
  PWMdirection = direction_;

  
}
// ***********************   JUNK   ***********************

// void SteeringStartpwm(Steering* car, Log* Log)
// {
//   
// 	
// 	
// //      std::cout << "startpwm() called: Thread starting:  "  << std::endl;
// //      Log_->writeEvent("SteeringStartpwm", "thread entered");
// 	
//       
//       while(1);
//       {
// 	car->PWMUpdate();
// 	
//       }
// }









/*void Steering::startpwm()

{	logPtr_->writeEvent("Steering startpwm", "start thread1");
  
	try {
	std::cout << "startpwm() called: Thread starting:  "  << std::endl;
	the_thread = std::thread(&Steering::PWMUpdate,this); 
	}
	catch (std::exception &e) 
	{
		logPtr_->writeEvent("Steering startpwm catch", e.what());
		std::cout<<"Caught exception: "<<e.what()<<"\n";
	}
	catch(const std::overflow_error& e) 
	{
	  logPtr_->writeEvent("Steering startpwm catch", e.what());
	  std::cout<<"Caught exception: overflow_error  "<<e.what()<<"\n";
	} 
	catch(const std::runtime_error& e) 
	{
	  logPtr_->writeEvent("Steering startpwm catch", e.what());
	  std::cout<<"Caught exception: runtime_error "<<e.what()<<"\n";
	}
	logPtr_->writeEvent("Steering startpwm", "start thread2");
}
*/