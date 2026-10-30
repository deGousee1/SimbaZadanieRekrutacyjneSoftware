#include <iostream>
#include <ostream>
#include "Test.h"

int main() {
    // Prośba o podanie numeru testu lub kodu wyjścia
    int testID;
    std::cout << "Input 1 for a flight test without simulated failures" << std::endl;
    std::cout << "Input 2 for a flight test with a simulated baro failure" << std::endl;
    std::cout << "Input 3 for a buffer test" << std::endl;
    std::cout << "Input 4 for a checksum error flag test" << std::endl;
    std::cout << "Input 0 for program exit" << std::endl;
    while (true) {
        // Obsługa błędu dla nieprzewidzianych wejść
        while (!(std::cin >> testID)) {
            std::cout << "Please input a number" << std::endl;

            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        // Wywoływanie odpowiednich testów i kodu wyjścia
        if (testID == 1) {
            Test::testFlightNoFailure();
        } else if (testID == 2) {
            Test::testFlightBaroError();
        } else if (testID == 3) {
            Test::bufferTest();
        } else if (testID == 4) {
            Test::checksumTest();
        } else if (testID == 0) {
            return 0;

        // Obsługa nieprzewidywanych liczb
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