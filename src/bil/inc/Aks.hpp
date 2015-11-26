#ifndef AKS_HPP
#define AKS_HPP

#include <Data.hpp>
#include <Steering.hpp>
#include <Settings.hpp>

enum aksStates{
    fwd,
    bwd,
    coasting,
    still
};

class Aks{
public:
    Aks(Data*, Settings*);
    void activate(void);

private:
    Data* MyData;
    Settings* MySettings;
    Steering MySteering;
    aksStates state;
};


#endif