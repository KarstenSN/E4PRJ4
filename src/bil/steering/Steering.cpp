#include <Steering.hpp>

//#define DEBUG_STEERING //Uncomment to enable debugging in steering class only

//----------Steering::Steering1----------
Steering::Steering(Data* dataClassPtr, Settings* MySettingsPtr, Log* MyLogPtr)
{
	dataClassPtr_ = dataClassPtr;
	settingsPtr_ = MySettingsPtr;
	logPtr_ = MyLogPtr;
	stop_thread = false;
	this->logPtr_->writeEvent(__PRETTY_FUNCTION__, "Steering constructor created");
	//***** Hardware PWM *****
	if (wiringPiSetupGpio() == -1)// Initialize wiringPi -- using Broadcom pin numbers
	{
		this->logPtr_->writeError(__PRETTY_FUNCTION__, "Failed to init wiringPiSetupGpio()");
	}
	pinMode(PWM_MOTOR_PIN, PWM_OUTPUT);// PWM_OUTPUT  /  OUTPUT
	pwmSetRange(PWM_SET_RANGE_VALUE); //The default is 1024.
	pwmSetClock(PWM_CLOCK_FREQ/(PWM_SET_RANGE_VALUE*PWM_FREQ)); //This sets the divisor for the PWM clock
	pwmSetMode(PWM_MODE_MS); //default mode in the Pi is "balanced" PWM_MODE_BAL PWM_MODE_MS 
	pinMode(PWM_MOTOR_FORWARD, OUTPUT);
	pinMode(PWM_MOTOR_BACKWARD, OUTPUT);
	//*****  PID *****
	iGain_ = iGain; // integral gain
	pGain_ = pGain; // proportional gain
	dGain_ = dGain; // derivative gain
	iMax_ = iMax;
	iMin_ = iMin;
	minServoPWM_ = minServoPWM;
	maxServoPWM_ = maxServoPWM;
	//***** Software PWM *****
	err = softPwmCreate(PWM_SERVO_PIN, 0, 200);
	if (err != 0)
		this->logPtr_->writeError(__PRETTY_FUNCTION__, "Failed to init softPwmCreate()");
	this->motorPWMThread = std::thread(&Steering::PWMUpdate , this);
	this->logPtr_->writeEvent(__PRETTY_FUNCTION__, "Constructor complete");
}
//----------Steering::Steering2----------


//----------Steering::~Steering1----------

Steering::~Steering()
{
	stop_thread = true;
	if (motorPWMThread.joinable())
		motorPWMThread.join();
	softPwmWrite(PWM_SERVO_PIN, 0);
	softPwmStop(PWM_SERVO_PIN);
	pwmWrite(PWM_MOTOR_PIN, 0);
	digitalWrite(PWM_MOTOR_FORWARD, LOW);
	digitalWrite(PWM_MOTOR_BACKWARD, LOW);
	
	this->logPtr_->writeEvent(__PRETTY_FUNCTION__, "DeConstructor");
}
//----------Steering::~Steering2----------


/**
* @param UsrInput_->forward  Den ønskede hastighed fremad  0..255. 255 er max af den indstillede hastighed
* @param UsrInput_->reverse Den ønskede hastighed bagud  0..255. 255 er max af den indstillede hastighed
* @param UsrInput_->turn Den ønskede dreje retning på forhjul. -127..127. -127 = fuldt udslag til venstre, center = 0, fuldt udslag til højre 127
* @param UsrInput_->stop Der skal bremses. Brems ikke = 0, Brems = 1.
*/

//----------Steering::userInput1----------

int Steering::userInput(UserInput* UsrInput_)
{
	this->max_speed_ = (int)settingsPtr_->getMaxSpeed();
	if (UsrInput_->stop == 1)
		this->brake();
	else
	{
		{
			std::lock_guard<std::mutex> lock(changeVar_Mut);
			speedReqFor_ = (int)((max_speed_*UsrInput_->forward) / 255); 	//(maxSpeed/255)*forward adjust input with max speed 
			speedReqBack_ = (int)((max_speed_*UsrInput_->reverse) / 255);	//(maxSpeed/255)*reverse adjust input with max speed
						
#ifdef DEBUG_STEERING
			fflush(stdout);
			std::cout << "userInput()" << "speedReqFor_ " << static_cast<int>(UsrInput_->forward) << " speedReqBack_ " << static_cast<int>(UsrInput_->reverse) << "            " << "\r";
#endif
		}
		this->motorSetPWM(UsrInput_->forward, UsrInput_->reverse);
		if (UsrInput_->forward < 5 && UsrInput_->reverse < 5)
		{
			this->softbrake();
		}
	}
	this->turn(UsrInput_->turn);
	return 1;
}
//----------Steering::userInput2----------


//----------Steering::brake1----------

int Steering::brake()
{
	std::lock_guard<std::mutex> lock(changeVar_Mut);
	activatePWM_ = 0;
	pwmWrite(PWM_MOTOR_PIN, PWM_SET_RANGE_VALUE);
	digitalWrite(PWM_MOTOR_FORWARD, LOW);
	digitalWrite(PWM_MOTOR_BACKWARD, LOW);
	return 1;
}
//----------Steering::brake2----------


//----------Steering::softbrake1----------

int Steering::softbrake()
{
	std::lock_guard<std::mutex> lock(changeVar_Mut);
	activatePWM_ = 0;
	pwmWrite(PWM_MOTOR_PIN, 0);
	digitalWrite(PWM_MOTOR_FORWARD, LOW);
	digitalWrite(PWM_MOTOR_BACKWARD, LOW);
	return 1;
}
//----------Steering::softbrake2----------


//----------Steering::turn1----------

int Steering::turn(signed char value)
{
	int TurnValue_ = ((((value + 128) * (maxServoPWM - minServoPWM)) / 255) + 5);
	softPwmWrite(PWM_SERVO_PIN, TurnValue_);
		
#ifdef DEBUG_STEERING
	std::string msg;			
	msg.append("Turn value received: ").append(std::to_string(value)).append(" Output value: ").append(std::to_string(TurnValue_));
	fflush(stdout);
	std::cout << "Steering::turn" << msg  << "            " << "\r";
#endif
	return 1;
}
//----------Steering::turn2----------


//----------Steering::motorSetPWM1----------

int Steering::motorSetPWM(unsigned char speedForward, unsigned char speedBackward)
{	
	std::lock_guard<std::mutex> lock(changeVar_Mut);
	speedAct_ = dataClassPtr_->getLatestVelocity();
	if (speedForward >= 0)
	{
		if (speedAct_ > 5 && direction_ == 0)
		{
			this->brake();
			return 1;
		}
		digitalWrite(PWM_MOTOR_FORWARD, HIGH);
		digitalWrite(PWM_MOTOR_BACKWARD, LOW);
		direction_ = 1;
		activatePWM_ = 1;
	}
	if (speedBackward > 0)
	{
		if (speedAct_ > 5 && direction_ == 1)
		{
			this->brake();
			return 1;
		}
		digitalWrite(PWM_MOTOR_FORWARD, LOW);
		digitalWrite(PWM_MOTOR_BACKWARD, HIGH);
		direction_ = 0;
		activatePWM_ = 1;
	}
	return 1;
}
//----------Steering::motorSetPWM2----------


//----------Steering::PWMUpdate1----------

void Steering::PWMUpdate()
{
	this->logPtr_->writeEvent(__PRETTY_FUNCTION__, "Thread starting");
	std::this_thread::sleep_for(std::chrono::microseconds(100000));
	while (!this->stop_thread)
	{	
	 	std::lock_guard<std::mutex> lock(changeVar_Mut);
		this->speedAct_ = this->dataClassPtr_->getLatestVelocity();
		// PID regulation
		if (direction_ == 1)
		{
            this->error_ = this->speedReqFor_; //- this->speedAct_; 
		}
		else
		{
            this->error_ = this->speedReqBack_; //- this->speedAct_;
		}
		pTemp_ = pGain_ * error_;		// calculate the proportional temp
		iState_ += error_;			// calculate the integral state with appropriate limiting
		if (iState_ > iMax_)
		{
			iState_ = iMax_;
		}
		else if (iState_ < iMin_)
		{
			iState_ = iMin_;
		}
		//iTemp_ = iGain_ * iState_; // calculate the integral temp
		//dTemp_ = dGain_ * (dState_ - speedAct_); // calculate the  derivative temp
		dState_ = speedAct_;
		motorPWMOutValue = pTemp_ + dTemp_ + iTemp_;
		if (motorPWMOutValue <= 0)
		{
			motorPWMOutValue = 0;
		}
		if (motorPWMOutValue >= PWM_SET_RANGE_VALUE)
		{
			motorPWMOutValue = PWM_SET_RANGE_VALUE;
		}
		if (activatePWM_ == 1)
		{
			pwmWrite(PWM_MOTOR_PIN, motorPWMOutValue);
		}
		std::this_thread::sleep_for(std::chrono::microseconds(2500)); // For testing. Needed ??
	}
	this->logPtr_->writeEvent(__PRETTY_FUNCTION__, "Thread stopping");
}
//----------Steering::PWMUpdate2----------
