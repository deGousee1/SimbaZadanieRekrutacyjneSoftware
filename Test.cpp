#include "Test.h"
#include <bitset>
#include <iostream>
#include <ostream>
#include "TelemetryBlackBox.h"


void Test::testFlightBaroError() {
    std::cout << "Test flight with baro error" << std::endl;
    TelemetryBlackBox box;
    // Symulacja lotu z awarią barometru na powrocie
    box.onRocketStateChange(DISARM);
    box.onBaroReading(false, 200); // Wysokość powinna zostać wyzerowana
    box.recordCurrentState();
    box.getLastDataframe();

    box.onRocketStateChange(ARM); // Sprawdzenie stanu ARM
    box.recordCurrentState();
    box.getLastDataframe();

    box.onRocketStateChange(FLIGHT); // Start
    for (int i = 0; i < 24; i++) {
        box.onBaroReading(false, (250 + i*1000));
        box.recordCurrentState();
        box.getLastDataframe();
    }
    box.onParachuteDeployed(); // Spadochron i opadanie
    box.onRocketStateChange(PARACHUTE);
    for (int i = 0; i < 24; i++) {
        if (i > 15) {
            box.onBaroReading(true, (23250 - i*1000));
        } else {
            box.onBaroReading(false, (23250 - i*1000));
        }
        box.recordCurrentState();
        box.getLastDataframe();
    }
    box.onRocketStateChange(LANDED);
    box.recordCurrentState();
    box.getLastDataframe();
}

void Test::testFlightNoFailure() {
    std::cout << "Test flight without failures (hopefully)" << std::endl;
    TelemetryBlackBox box;

    // Symulacja lotu bez awarii
    box.onRocketStateChange(DISARM);
    box.onBaroReading(false, 200); // Wysokość powinna zostać wyzerowana
    box.recordCurrentState();
    box.getLastDataframe();

    box.onRocketStateChange(ARM); // Sprawdzenie stanu ARM
    box.recordCurrentState();
    box.getLastDataframe();

    box.onRocketStateChange(FLIGHT); // Start
    for (int i = 0; i < 24; i++) {
        box.onBaroReading(false, (250 + i*1000));
        box.recordCurrentState();
        box.getLastDataframe();
    }
    box.onParachuteDeployed(); // Spadochron i opadanie
    box.onRocketStateChange(PARACHUTE);
    for (int i = 0; i < 24; i++) {
        box.onBaroReading(false, (23250 - i*1000));
        box.recordCurrentState();
        box.getLastDataframe();
    }
    box.onRocketStateChange(LANDED);
    box.recordCurrentState();
    box.getLastDataframe();
}

void Test::bufferTest() {
    TelemetryBlackBox box;
    box.onRocketStateChange(FLIGHT);

    // Zmuszenie bufora do nadpisania kilku elementów
    for (int i = 0; i < 22; i++) {
        box.recordCurrentState();
        box.onBaroReading(false, (5000 + i));
        box.getLastDataframe();
    }

    // Wydrukowanie ciałej pamięci bufora z wyciągniętą i przetłumaczoną wysokością dla łatwiejszej interpretacji testu
    for (int i = 0; i < 16; i++) {
        std::cout << i+1 << ". " << std::bitset<32>(box.buffer[i]) << " Altitude: " << ((box.buffer[i] >> 8) & 0b1111111111111111) << std::endl;
    }

    // Sprawdzenie czy elementy zostały nadpisane poprawnie przez porównanie różnicy elementu który powinien być najmłodszy i tego który powinien być najmłodszy. Gdyby tak nie było, różnica wynosiłaby 1, a nie 15.
    if (((box.buffer[5] >> 8) & 0b1111111111111111) - ((box.buffer[6] >> 8) & 0b1111111111111111) == 15) {
        std::cout << "Buffer test successful" << std::endl;
    } else {
        std::cout << "Buffer test failed" << std::endl;
    }
}

void Test::checksumTest() {
    // Utworzenie i zapisanie jednej ramki
    TelemetryBlackBox box;
    box.onRocketStateChange(DISARM);
    box.onBaroReading(false, 5000);
    box.recordCurrentState();
    // Przechwycenie utworzonej ramki i podmiana checksum na jej negację w celu przetestowania flagi błędu dla checksum
    uint8_t checksumT = ~(box.lastDataframe >> 24);
    uint32_t frameT = ((box.lastDataframe << 8) >> 8) | (checksumT << 24);

    // Wrzucenie podrobionej wadliwej ramki do bufora
    if (box.pointer == 16) {
        box.pointer = 0;
    }
    box.buffer[box.pointer] = frameT;
    box.pointer++;

    // Wywołanie funkcji w której jest logika sprawdzania wadliwego checksum
    box.getLastDataframe();

    // Program powinien zwrócić "Checksum error" i to już wystarczajacy rezultat testu, ale dla jasności dodałem jeszcze flagę zrobioną pod sam test
    // Sprawdzenie flagi testu checksum i wydrukowanie wyniku testu na konsoli
    if (box.checksumTestFlag == true) {
        std::cout << "Checksum error flag test successful" << std::endl;
    } else {
        std::cout << "Checksum error flag test failed" << std::endl;
    }
}