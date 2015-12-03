/*
 * main.c:
 *      Simple test program to test the wiringPi functions
 *      PWM test using "regular PWM mode"
 */

#include <wiringPi.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <stdint.h>

#define PWM_CLOCK_FREQ 19200000

/* Please set below values as wanted */
#define PWM_PIN_NO 18 		//Use "gpio readall" in terminal to find BCM value for desired pin #
#define PWM_FREQ 50000 		//The wanted PWM frequency in Hz
#define PWM_RANGE 128 		//Resolution of duty cycle

int main (void){
	std::cout << "Raspberry Pi wiringPi PWM test program" << std::endl;
	
	if (wiringPiSetupGpio() == -1)
		exit (1) ;
	
	for(int i = 0; i < 17; i++){
		pinMode (i, PWM_OUTPUT);
	}
	
	pwmSetRange(PWM_RANGE);
	pwmSetClock(PWM_CLOCK_FREQ/(PWM_RANGE*PWM_FREQ));
	pwmSetMode(PWM_MODE_MS);
	
	int count;
	
	while(1){
		pwmWrite (PWM_PIN_NO, count);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		count++;
		count = count % PWM_RANGE;
	}
	
	return 0 ;
}