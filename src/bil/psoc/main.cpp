#include <string>
#include <Psoc.hpp>

int main() {
    
    
    Log myLog;
    
    Psoc myPsoc(&myLog);

    for(int i = 0; i<5;i++){
      std::cout << myPsoc.getDistance("FL") << std::endl;
      std::cout << myPsoc.getVelocity() << std::endl;    
      
      sleep(1);
    }
    return 0;
}