#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <thread>
#include <mutex>
#include "Log.hpp"


Log::Log(std::string filename){
	
	logFile.open(filename, std::ofstream::out | std::ofstream::app);
	if(!logFile.is_open()){
		std::cout << "Warning: Logging not active!" << std::endl;
	}
	else{
		this->writeEvent(__PRETTY_FUNCTION__,"Log class active");
	}
}

bool Log::writeError(std::string from, std::string msg){
	std::lock_guard<std::mutex> lock(mutex);
	logFile << "[" << this->getTimestamp() << "] [Error] [" << from << "] " << msg << std::endl;
	return true;
}

bool Log::writeWarning(std::string from, std::string msg){
	std::lock_guard<std::mutex> lock(mutex);
	logFile << "[" << this->getTimestamp() << "] [Warning] [" << from << "] " << msg << std::endl;
	return true;
}

bool Log::writeEvent(std::string from, std::string msg){
	std::lock_guard<std::mutex> lock(mutex);
	logFile << "[" << this->getTimestamp() << "] [Event] [" << from << "] " << msg << std::endl;
	return true;
}

Log::~Log(){
	this->writeEvent(__PRETTY_FUNCTION__,"Log class shutdown");
	logFile << std::endl;
	logFile.close();
}

std::string Log::getTimestamp(){
	std::string timestamp;

	time( &this->currentTime );								// Get the current time
		this->localTime = localtime( &this->currentTime );	// Convert the current time to the local time

		int Day    = localTime->tm_mday;
		int Month  = localTime->tm_mon + 1;
		int Year   = localTime->tm_year + 1900;
		int Hour   = localTime->tm_hour;
		int Min    = localTime->tm_min;
		int Sec    = localTime->tm_sec;

		timestamp = std::to_string(Year) + "-" + std::to_string(Month) + "-" + std::to_string(Day) + " " + std::to_string(Hour) + ":" + std::to_string(Min) + ":" + std::to_string(Sec);

	return timestamp;
}