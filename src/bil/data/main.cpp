#include <iostream>
#include <thread>
#include <random>
#include "Data.hpp"
#include "Log.hpp"
#include "utilities.hpp"


#define NUM_THREADS 10

Log l;
Data d(&l);

void writers(int num){
	
	// Seeding random number
	std::random_device rd;
    std::mt19937 gen(rd());

	// Generate random numbers for sensor attributes
	std::uniform_int_distribution<> vel(0,200);
	int velocity = vel(gen);
	std::uniform_int_distribution<> acc(0,50);
	int acceleration = acc(gen);
	std::uniform_int_distribution<> dist(0,250);
	int distanceFR = dist(gen);

	// Writing to sensor attributes
	d.writeVelocity(velocity);
	d.writeAcceleration(acceleration);
	d.writeDistance("FR",distanceFR);

	// Generate random numbers for user attributes
	std::uniform_int_distribution<> inp(1,255);
	UserInput Input{static_cast<char>(inp(gen)),static_cast<char>(inp(gen)),static_cast<char>(inp(gen)),static_cast<char>(inp(gen))};

	// Writing to user attributes
	d.writeUserInput(&Input);

	std::string msg = "Writer thread #" + std::to_string(num) +
		": velocity=" + std::to_string(velocity) + " acceleration=" +
		std::to_string(acceleration) + " distanceFR=" + std::to_string(distanceFR) +
		" userInput=" + std::to_string(static_cast<int>(Input.forward)) + " " +
		std::to_string(static_cast<int>(Input.reverse)) + " " +
		std::to_string(static_cast<int>(Input.turn)) + " " + std::to_string(static_cast<int>(Input.stop));

	l.writeEvent(__PRETTY_FUNCTION__,msg);
	
}

void reader(int num){

	// Reading sensor attributes
	std::string vel = std::to_string(d.getLatestVelocity());
    std::string acc = std::to_string(d.getLatestAcceleration());
    std::string dist = std::to_string(d.getLatestDistance("FR"));

    // Reading user attributes
    UserInput Input = d.getUserInput();
    std::string forw = std::to_string(static_cast<int>(Input.forward));
    std::string reve = std::to_string(static_cast<int>(Input.reverse));
    std::string turn = std::to_string(static_cast<int>(Input.turn));
    std::string stop = std::to_string(static_cast<int>(Input.stop));

    // Writing to log
	std::string msg = "Reader thread #" + std::to_string(num) + ": velocity=" + vel + " acceleration=" +
		acc + " distanceFR=" + dist + " userInput=" + forw + " " + reve + " " + turn + " " + stop;
	l.writeEvent(__PRETTY_FUNCTION__,msg);

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