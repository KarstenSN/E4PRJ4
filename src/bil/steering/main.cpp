
#include <Steering.hpp>
#include <wiringPi.h>
#include <Data.hpp>
#include <Settings.hpp>
#include <Log.hpp>
#include <string>

int main()
{	
	Log loginfo;
	Data data(&loginfo);
        data.writeVelocity(100);
	
	std::cout << " Main: Data loaded " << std::endl;
	Settings set(&loginfo);
	
	set.setMaxSpeed(50);
	
	
	std::cout << " Main: making car XXXXX " << std::endl;

	Steering car(&data, &set, &loginfo);
	std::cout << " Main: made car " << std::endl;

	UserInput testuser;
 	testuser.forward = 0;
 	testuser.reverse = 0;
 	testuser.stop = 0;
 	testuser.turn = 0;
 	
	std::this_thread::sleep_for(std::chrono::seconds(5));
	
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
	std::cout << "Main Turn input (-127) : " << testuser.turn << std::endl;
	car.userInput(&testuser);
	
	std::this_thread::sleep_for(std::chrono::seconds(5));
	testuser.forward = 150;
	testuser.reverse = 0;
	testuser.turn = (char)-90;
	std::cout << "Main Turn input(-90) : " << testuser.turn << std::endl;
	car.userInput(&testuser);
	
	std::this_thread::sleep_for(std::chrono::seconds(5));
	testuser.forward = 0;
	testuser.reverse = 0;
	testuser.turn = (char)-40;
	std::cout << "Main Turn input(-40): " << testuser.turn << std::endl;
	car.userInput(&testuser);
	
	std::this_thread::sleep_for(std::chrono::seconds(5));
	testuser.forward = 150;
	testuser.reverse = 0;
	testuser.turn = (char)0;
	std::cout << "Main Turn input(0): " << testuser.turn << std::endl;
	car.userInput(&testuser);
	
	std::this_thread::sleep_for(std::chrono::seconds(5));
	testuser.forward = 0;
	testuser.reverse = 150;
	data.writeVelocity(0);
	testuser.reverse = 150;
	testuser.turn =(char) 40;
	std::cout << "Main Turn input(40): " << testuser.turn << std::endl;
	car.userInput(&testuser);
	
	std::this_thread::sleep_for(std::chrono::seconds(5));
	testuser.forward = 250;
	testuser.reverse = 0;
	testuser.turn = (char)90;
	std::cout << "Main Turn input(90): " << testuser.turn << std::endl;
	car.userInput(&testuser);
	
	std::this_thread::sleep_for(std::chrono::seconds(5));
	testuser.forward = 255;
	testuser.reverse = 0;
	testuser.turn = (char)127;
	std::cout << "Main Turn input(127): " << testuser.turn << std::endl;
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