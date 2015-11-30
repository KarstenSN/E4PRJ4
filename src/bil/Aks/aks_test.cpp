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
	
	
	AksActivateTh.join();
	
	return 0;
}