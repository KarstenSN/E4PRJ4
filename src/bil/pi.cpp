#include <iostream>
#include <Data.hpp>
#include <PcCom.hpp>
#include <Aks.hpp>
#include <Settings.hpp>
#include <Steering.hpp>

int main(){
    Data Data_;
    Settings Settings_;
    Aks Aks_(&Data_, &Settings_);
    PcCom PcCom_(&Data_, &Settings_);
    
    Aks_.activate();
    
    
    
    return 0;
}