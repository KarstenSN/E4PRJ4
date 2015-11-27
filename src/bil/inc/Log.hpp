#ifndef _INCL_LOG_HPP
#define _INCL_LOG_HPP

#include <fstream>

class Log{
public:

	Log(std::string filename = "Log.txt");
	bool writeError(std::string from, std::string msg);
	bool writeWarning(std::string from, std::string msg);
	bool writeEvent(std::string from, std::string msg);
	~Log();

private:
	std::ofstream logFile;

	pthread_mutex_t mutex;

	time_t currentTime;
	struct tm *localTime;

	std::string getTimestamp();

};

#endif