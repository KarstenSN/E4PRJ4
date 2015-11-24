#include <iostream>
#include <Data.hpp>
#include <PcCom.hpp>
#include <Aks.hpp>

int main(){
    Data MyData;
    Aks TheAks(&MyData);
    
    TheAks.start();
    
    return 0;
}