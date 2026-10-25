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
    std::cout << "Input 0 for program exit" << std::endl;
    while (true) {
        while (!(std::cin >> testID)) {
            std::cout << "Please input a number" << std::endl;

            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
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
        } else if (testID == 0) {
            return 0;
        } else {
            std::cout << "Program " << testID << " doesn't exist. Please input a valid test ID" << std::endl;
            std::cout << "1. Flight test no failures" << std::endl;
            std::cout << "2. Flight test baro failure" << std::endl;
            std::cout << "3. Buffer test" << std::endl;
            std::cout << "4. Checksum error flag test" << std::endl;
            std::cout << "0. Program exit" << std::endl;
        }
    }
}