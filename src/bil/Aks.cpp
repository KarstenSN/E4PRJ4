#include <Aks.hpp>
#include <utilities.hpp>
#include <Data.hpp>
#include <Settings.hpp>
#include <Steering.hpp>


Aks::Aks(Data* Dataptr, Settings* NewSettings): MySteering(NewSettings) {
    this->MyData = Dataptr;
    this->MySettings = NewSettings;
}

void Aks::start(){
        
}