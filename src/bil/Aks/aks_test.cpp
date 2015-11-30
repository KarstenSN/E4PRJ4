#include <iostream>
#include <thread>
#include <Log.hpp>
#include <Aks.hpp>
#include <Settings.hpp>
#include <Data.hpp>

int main(){
    Log myLog;
	Data myData(&myLog);
	Settings mySettings;
	Aks myAks(&myData, &mySettings, &myLog);
	
	std::thread AksActivateTh(&Aks::activate, &myAks);
	
	UserInput myUserInput;
	myUserInput.forward = 100;
	
	myData.writeUserInput(&myUserInput);
	
	char kbIn = 0;
	while(kbIn != 'e'){
		std::cout << "Write a, w, s, d or b for inputs, e to exit:" << std::endl;
		std::cin >> kbIn;
		std::cout << std::endl;
		switch(kbIn){
			case 'w':
				myData.writeUserInput(&(myUserInput = UserInput(100, 0, 0, 0)));
				break;
			case 's':
				myData.writeUserInput(&(myUserInput = UserInput(0, 100, 0, 0)));
				break;
			case 'a':
				myData.writeUserInput(&(myUserInput = UserInput(0, 0,-100, 0)));
				break;
			case 'd':
				myData.writeUserInput(&(myUserInput = UserInput(0, 0, 100, 0)));
				break;
			case 'b':
				myData.writeUserInput(&(myUserInput = UserInput(0, 0, 0, 1)));
				break;
			case 'e':
				exit(0);
				break;
			default:
				std::cout << "Please input valid chars!" << std::endl;
				break;
		}
	}
	
	return 0;
}