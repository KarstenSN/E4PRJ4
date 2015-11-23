#include <iostream>
#include "PcCom.hpp"
#include "Data.hpp"
#include "Settings.hpp"

int main(int argc, char *argv[]) {

    settingsTest setTest;
    dataTest datTest;

    PcCom pcTest(&datTest, &setTest);

    pcTest.dataStream();
    return 0;
}
