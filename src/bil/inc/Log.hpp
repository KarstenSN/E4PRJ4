#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>


class Log{
public:
	Log(std::string filename){
		this->filename = filename;
		logFile.open(filename, std::ofstream::out | std::ofstream::app);
		
		time_t t = time(0);   // get time now
    	struct tm * now = localtime( & t );
    	std::cout << (now->tm_year + 1900) << '-' 
         << (now->tm_mon + 1) << '-'
         <<  now->tm_mday
         << std::endl;

		std::cout << t << ": Log with filename '" << filename << "' created." << std::endl;
	}

	bool writeError(std::string from, std::string msg){
		logFile << "Error at " << from << ": " << msg << std::endl;
		return true;
	}

	bool writeWarning(std::string from, std::string msg){
		logFile << "Warning at " << from << ": " << msg << std::endl;
		return true;
	}

	bool writeEvent(std::string from, std::string msg){
		logFile << "Event at " << from << ": " << msg << std::endl;
		return true;
	}

	~Log(){
		logFile.close();
	}

private:
	std::string filename;
	std::ofstream logFile;

};

#endif