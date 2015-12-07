#include <string>
#include <Psoc.hpp>

int main() {
    
    
    Log myLog;
    
    Psoc myPsoc(&myLog);
    
    std::cout << "Class created" << std::endl;
    
    //myPsoc.getDistance("FL");   
    //myPsoc.getVelocity();
    
    sleep(2);
    
    return 0;
}
 
