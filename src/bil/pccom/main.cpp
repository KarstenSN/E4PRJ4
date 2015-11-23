#include <iostream>
#include "PcCom.h"
#include "Data.h"
#include "Settings.h"

int main(int argc, char *argv[]) {

    settingsTest setTest;
    dataTest datTest;

    pcCom pcTest(&datTest, &setTest);

    pcTest.dataStream();
    return 0;
}
