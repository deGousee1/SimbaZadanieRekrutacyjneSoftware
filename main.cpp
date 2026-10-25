#include <iostream>
#include <ostream>

#include "TelemetryBlackBox.h"
#include "Test.h"

int main() {
    int testID;
    std::cout << "Input 1 for a flight test without simulated failures" << std::endl;
    std::cout << "Input 2 for a flight test with a simulated baro failure" << std::endl;
    std::cout << "Input 3 for a buffer test" << std::endl;
    std::cout << "Input 4 for a checksum error flag test" << std::endl;
    std::cin >> testID;
    if (testID == 1) {
        Test test;
        test.testFlightNoFailure();
    } else if (testID == 2) {
        Test test;
        Test::testFlightBaroError();
    } else if (testID == 3) {
        Test test;
        Test::bufferTest();
    } else if (testID == 4) {
        Test test;
        Test::checksumTest();
    }

    return 0;
}