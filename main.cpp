#include "TelemetryBlackBox.h"

int main() {
    TelemetryBlackBox box; // Utworzenie obiektu czarnej skrzynki
    box.onRocketStateChange(DISARM);
    box.onBaroReading(false, 200); // Wysokość powinna zostać wyzerowana
    box.recordCurrentState();
    box.getLastDataframe();

    box.onRocketStateChange(ARM); // Sprawdzenie stanu ARM
    box.recordCurrentState();
    box.getLastDataframe();

    // Symulacja lotu z awarią barometru na powrocie
    box.onRocketStateChange(FLIGHT);
    for (int i = 0; i < 24; i++) {
        box.onBaroReading(false, (250 + i*1000));
        box.recordCurrentState();
        box.getLastDataframe();
    }
    box.onParachuteDeployed();
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

    // Symulacja lotu bez awarii
    box.onRocketStateChange(FLIGHT);
    for (int i = 0; i < 24; i++) {
        box.onBaroReading(false, (250 + i*1000));
        box.recordCurrentState();
        box.getLastDataframe();
    }
    box.onParachuteDeployed();
    box.onRocketStateChange(PARACHUTE);
    for (int i = 0; i < 24; i++) {
        box.onBaroReading(false, (23250 - i*1000));
        box.recordCurrentState();
        box.getLastDataframe();
    }
    return 0;
}