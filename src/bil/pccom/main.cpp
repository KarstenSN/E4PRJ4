#include <iostream>
#include "PcCom.hpp"
#include "Data.hpp"
#include "Settings.hpp"

//----------PcCom::testprogram1----------
int main(int argc, char *argv[]) {

    settingsTest setTest;
    dataTest datTest;

    PcCom pcTest(&datTest, &setTest);

    pcTest.dataStream();
    return 0;
}
//----------PcCom::testprogram2----------