#include <string>
#include <Psoc.hpp>

int main() {
    
    
    Log myLog;
    
    Psoc myPsoc(&myLog);

    while(1){
      std::cout << "FL: " << myPsoc.getDistance("FL") << std::endl;
      std::cout << "FR: " << myPsoc.getDistance("FR") << std::endl;
      std::cout << "RL: " << myPsoc.getDistance("RL") << std::endl;
      std::cout << "RR: " << myPsoc.getDistance("RR") << std::endl;
      std::cout << "Km/t: " << ((double)myPsoc.getVelocity())/10 << std::endl;    
      
      usleep(100000);
    }
    return 0;
}