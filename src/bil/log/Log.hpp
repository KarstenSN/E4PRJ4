#ifndef _INCL_LOG_HPP
#define _INCL_LOG_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <pthread.h>
#include "utilities.hpp"

class Log{
public:

	Log(std::string filename = "Log.txt"){
		
		logFile.open(filename, std::ofstream::out | std::ofstream::app);
		if(!logFile.is_open()){
			std::cout << "Warning: Logging not active!" << std::endl;
		}
	}

	bool writeError(std::string from, std::string msg){
		ScopedLocker Lock(&mutex);
		logFile << "[" << this->getTimestamp() << "] [Error] [" << from << "] " << msg << std::endl;
		return true;
	}

	bool writeWarning(std::string from, std::string msg){
		ScopedLocker Lock(&mutex);
		logFile << "[" << this->getTimestamp() << "] [Warning] [" << from << "] " << msg << std::endl;
		return true;
	}

	bool writeEvent(std::string from, std::string msg){
		ScopedLocker Lock(&mutex);
		logFile << "[" << this->getTimestamp() << "] [Event] [" << from << "] " << msg << std::endl;
		return true;
	}

	~Log(){
		logFile.close();
	}

private:
	std::ofstream logFile;

	pthread_mutex_t mutex;

	time_t currentTime;
	struct tm *localTime;

	std::string getTimestamp(){
		std::string timestamp;

		time( &this->currentTime );							// Get the current time
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

};

#endif