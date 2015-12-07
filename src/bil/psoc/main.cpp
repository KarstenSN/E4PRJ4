#include <string>
#include <Psoc.hpp>

int main() {
    
    
    Log myLog;
    
    Psoc myPsoc(&myLog);
    
    myPsoc.getDistance("FL");   
    myPsoc.getVelocity();
    
    sleep(2);
    
    return 0;
}
 
