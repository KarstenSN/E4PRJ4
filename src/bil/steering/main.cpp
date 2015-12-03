
#include "Steering.hpp"
#include <wiringPi.h>
#include "Data.hpp"
#include "Settings.hpp"
#include "Log.hpp"
#include <string>

int main()
{	
	Log loginfo;
	Data data(&loginfo);
        data.writeVelocity(10);
	
	std::cout << " Main: Data loaded " << std::endl;
	Settings set;
	
	set.setMaxSpeed(50);
	
	
	std::cout << " Main: making car XXXXX " << std::endl;

	Steering car(&data, &set, &loginfo);
	std::cout << " Main: made car " << std::endl;

	UserInput testuser;
 	testuser.forward = 0;
 	testuser.reverse = 0;
 	testuser.stop = 0;
 	testuser.turn = 0;
 	
	car.userInput(&testuser);

	std::cout << " Main: Start pwm thread" << std::endl;
	std::thread motorPWMThread (&Steering::PWMUpdate,&car);
	std::cout << " Main: Thread started " << std::endl;
	//std::this_thread::sleep_for(std::chrono::seconds(5));
	
	//*****  TEST SEQUENCE *****
while(1)
{	
	
	std::this_thread::sleep_for(std::chrono::seconds(5));
	testuser.forward = 0;
	testuser.turn = -127;
	car.userInput(&testuser);
	
	std::this_thread::sleep_for(std::chrono::seconds(5));
	testuser.forward = 50;
	testuser.turn = -90;
	car.userInput(&testuser);
	
	std::this_thread::sleep_for(std::chrono::seconds(5));
	testuser.forward = 100;
	testuser.turn = -40;
	car.userInput(&testuser);
	
	std::this_thread::sleep_for(std::chrono::seconds(5));
	testuser.forward = 150;
	testuser.turn = 0;
	car.userInput(&testuser);
	
	std::this_thread::sleep_for(std::chrono::seconds(5));
	testuser.forward = 200;
	testuser.turn = 40;
	car.userInput(&testuser);
	
	std::this_thread::sleep_for(std::chrono::seconds(5));
	testuser.forward = 250;
	testuser.turn = 90;
	car.userInput(&testuser);
	
	std::this_thread::sleep_for(std::chrono::seconds(5));
	testuser.forward = 255;
	testuser.turn = 127;
	car.userInput(&testuser);
}	
	std::this_thread::sleep_for(std::chrono::seconds(5));
	
	
	car.stop_thread = 1;
	std::cout << " Main: End of test " << std::endl;
	motorPWMThread.join();
	while(1)
	{
	  
	}
		
	std::cout << " Main: End of program " << std::endl;	
	  
	
return 0;
} 