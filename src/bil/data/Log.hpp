#ifndef _INCL_LOG_HPP
#define _INCL_LOG_HPP

#include <fstream>
#include <mutex>


class Log{
public:

	Log(std::string filename = "pi-log.txt");
	bool writeError(std::string from, std::string msg);
	bool writeWarning(std::string from, std::string msg);
	bool writeEvent(std::string from, std::string msg);
	~Log();

private:
	std::ofstream logFile;

	std::mutex mutex;

	time_t currentTime;
	struct tm *localTime;

	std::string getTimestamp();

};

#endif