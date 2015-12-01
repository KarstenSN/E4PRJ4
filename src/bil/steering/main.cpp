
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
	
	set.setMaxSpeed(5);
	
	
	std::cout << " Main: making car " << std::endl;

	Steering car(&data, &set,&loginfo);
	std::cout << " Main: made car " << std::endl;

	UserInput testuser;
 	testuser.forward = 150;
 	testuser.reverse = 0;
 	testuser.stop = 0;
 	testuser.turn = 0;
 	
	car.userInput(&testuser);

	std::cout << " Main: startpwm " << std::endl;
	std::thread motorPWMThread (&Steering::PWMUpdate,&car);
	
	
	std::cout << " Main: startpwm started " << std::endl;
	motorPWMThread.join();
// while(1)
// {}
// 	
	
	
	
return 0;
} 