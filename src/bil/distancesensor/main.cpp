#include <iostream>
#include <unistd.h>
#include "DistanceSensor.hpp"
#include "Log.hpp"


int main(){

    std::cout << "Main running!" << std::endl;

    DistanceSensor sensorGruppe;

    for(int i = 0; i < 5; i++ ){
    std::cout << "#" << i << ": " << sensorGruppe.getDistance("FL") << std::endl;
    sleep(1);
    }

    return 0;
}
