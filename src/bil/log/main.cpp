#include <iostream>
#include <string>
#include "Log.hpp"
#include <thread>

#define NUM_THREADS 10

Log Log;

void writers(int num){
	
	std::string msg = "Thread #" + std::to_string(num) + "checking in..";

	Log.writeEvent(__PRETTY_FUNCTION__,msg);
}


int main()
{

	std::thread myThreads[NUM_THREADS];
	
    for(int i = 0; i < NUM_THREADS; i++){
        myThreads[i] = std::thread(writers, i);
    }

	for(int i = 0; i < NUM_THREADS; i++)
	{
		myThreads[i].join();

	}
	return 0;
}