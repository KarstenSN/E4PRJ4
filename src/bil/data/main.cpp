#include <iostream>
#include <thread>
#include <ctime>
//#include <cstdlib>
#include "Data.hpp"
#include "Log.hpp"


#define NUM_THREADS 2

Log l;
Data d(&l);

void writers(int num){
	
	// Random seed
	srand (time(NULL));

	// Generate random numbers
	int velocity = rand() % 200;
	int acceleration = rand() % 50;
	int distanceFR = rand() % 250;

	// Writing to attributes
	d.writeVelocity(velocity);
	d.writeAcceleration(acceleration);
	d.writeDistance("FR",distanceFR);

	// TODO write to user inputs

	std::string msg = "Writer thread #" + std::to_string(num) +
		": velocity=" + std::to_string(velocity) + " acceleration=" +
		std::to_string(acceleration) + " distanceFR=" + std::to_string(distanceFR);

	l.writeEvent(__PRETTY_FUNCTION__,msg);
	
	sleep(1);
}

void reader(int num){

	// TODO make readings
	//std::string msg = "Reader thread #" + std::to_string(num) + " checking in..";
	//l.writeEvent(__PRETTY_FUNCTION__,msg);
	//sleep(1);
}


int main(){
    
    std::cout << "Main running" << std::endl;
    
    std::thread myWriters[NUM_THREADS];
    std::thread myReaders[NUM_THREADS];
	
    for(int i = 0; i < NUM_THREADS; i++){
        myWriters[i] = std::thread(writers, i);
        myReaders[i] = std::thread(reader, i);
    }

    std::cout << "Waiting for treads to join" << std::endl;

	for(int i = 0; i < NUM_THREADS; i++){
		myWriters[i].join();
		myReaders[i].join();
	}

    std::cout << "Main shutdown" << std::endl;
    return 0;
}