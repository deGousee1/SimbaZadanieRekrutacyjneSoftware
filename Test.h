#ifndef SIMBAREKRUTACYJNESOFTWARE_TEST_H
#define SIMBAREKRUTACYJNESOFTWARE_TEST_H
#include "TelemetryBlackBox.h"


class Test {
    public:

    static void testFlightNoFailure();

    static void testFlightBaroError();

    static void bufferTest();

    static void checksumTest();
};


#endif //SIMBAREKRUTACYJNESOFTWARE_TEST_H
