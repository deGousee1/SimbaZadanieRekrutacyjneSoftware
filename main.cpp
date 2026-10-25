#include "TelemetryBlackBox.h"

int main() {
    TelemetryBlackBox box; // Utworzenie obiektu czarnej skrzynki
    for (int i = 0; i < 32; i++) {
        box.onRocketStateChange(FLIGHT);
        box.onBaroReading(false, 5000);
        box.recordCurrentState();
        box.getLastDataframe();
        box.onRocketStateChange(FLIGHT);
        box.onBaroReading(true, 6000);
        box.recordCurrentState();
        box.getLastDataframe();
    } // Prymitywny kod testowy wywołujący poszczególne funkcje
    return 0;
}