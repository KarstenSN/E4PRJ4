#include <iostream>
#include <Data.hpp>
#include <PcCom.hpp>
#include <Aks.hpp>
#include <Settings.hpp>
#include <Steering.hpp>
#include <Log.hpp>

int main(){
    Settings Settings_;
	Log Log_("pilog.txt");
    Data Data_(&Log_);
    Aks Aks_(&Data_, &Settings_, &Log_);
    PcCom PcCom_(&Data_, &Settings_, &Log_);
    
	Log_.writeEvent("pi.cpp", "System initialized.");
    Aks_.activate();
    
    
    
    return 0;
}