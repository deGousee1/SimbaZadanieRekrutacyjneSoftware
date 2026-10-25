#include "TelemetryBlackBox.h"
#include <iostream>
#include <bitset>
#include <string> // Inkludy

void TelemetryBlackBox::onRocketStateChange(RocketState state) {
    state_ = state; // Zapisanie stanu fazy lotu
}

void TelemetryBlackBox::onBaroReading(bool error, uint16_t value) {
    baroError = error;
    altitude = value; // Zapisanie stanu wysokości i wcześniej sprawdzenia czy barometr ma jakiś błąd
}

void TelemetryBlackBox::onParachuteDeployed() {
    parachute = true; // Zapisanie stanu otwarcia spadochronu
}

void TelemetryBlackBox::recordCurrentState() {
    // Deklaracje koniecznych zmiennych
    uint16_t alt = altitude;
    uint8_t byte0 = 0;
    uint8_t byte1 = 0;
    uint8_t byte2 = 0;
    uint8_t checksum = 0;
    uint32_t frame = 0;

    // Implementacja koniecznej logiki
    if (state_ == RocketState::DISARM or state_ == RocketState::LANDED) {
        alt = 0;
    }
    if (baroError == true) {
        alt = 0b1111111111111111;
    }
    // Tworzenie ramki
    byte0 = (state_ & 0b111) | (parachute << 3) | (baroError << 4);
    byte1 = alt & 0b11111111;
    byte2 = (alt >> 8) & 0b11111111;
    checksum = byte0 ^ byte1 ^ byte2;
    frame = (checksum << 24) | (byte2 << 16) | (byte1 << 8) | byte0;

    // Zawijanie i przesuwanie bufora jak i zapisywanie ramki
    if (pointer == 16) {
        pointer = 0;
    }
    buffer[pointer] = frame;
    pointer++;
}

uint32_t TelemetryBlackBox::getLastDataframe() {
    // Pobranie najnowszej ramki
    if (pointer == 0) {
        return 0xFFFFFFFF;
    }
    lastDataframe = buffer[pointer-1];
    // Ponowne obliczenie checksum
    uint8_t byte0_ = lastDataframe & 0b11111111;
    uint8_t byte1_ = (lastDataframe >> 8) & 0b11111111;
    uint8_t byte2_ = (lastDataframe >> 16) & 0b11111111;
    uint8_t checksumC = lastDataframe >> 24;
    uint8_t checksum_ = byte0_ ^ byte1_ ^ byte2_;

    // Oddzielenie i przetłumaczenie stanu na tekst
    uint8_t stateCout = lastDataframe & 0b111;
    std::string stateFinal;
    if (stateCout == 0) {
        stateFinal = "DISARM";
    } else if (stateCout == 1) {
        stateFinal = "ARM";
    } else if (stateCout == 2) {
        stateFinal = "FLIGHT";
    } else if (stateCout == 3) {
        stateFinal = "PARACHUTE";
    } else if (stateCout == 4) {
        stateFinal = "LANDED";
    } else {
        stateFinal = "ERROR";
    }

    // Oddzielenie i przetłumaczenie stanu spadochronu na tekst
    uint8_t parachuteCout = (lastDataframe >> 3) & 0b1;
    std::string parachuteFinal;
    if (parachuteCout == 0) {
        parachuteFinal = "CLOSED";
    } else {
        parachuteFinal = "DEPLOYED";
    }

    // Oddzielenie i przetłumaczenie wartości błędu barometru na tekst
    uint8_t baroErrorCout = (lastDataframe >> 4) & 0b1;
    std::string baroErrorFinal;
    if (baroErrorCout == 0) {
        baroErrorFinal = "VALID";
    } else {
        baroErrorFinal = "ERROR";
    }

    // Oddzielenie reserved i wysokości
    uint8_t reservedCout = (lastDataframe >> 5) & 0b111;
    uint16_t altitudeCout = (lastDataframe >> 8) & 0b1111111111111111;

    // Cout na konsolę
    std::cout << "State: " << stateFinal << "| Parachute: " << parachuteFinal << "| Baro: " << baroErrorFinal << "| Reserved: " << std::bitset<3>(reservedCout) << "| Altitude: " << altitudeCout << " feet" << " Checksum: " << std::bitset<8>(checksumC) << std::endl;

    // Return ostatniej ramki lub kodu błędu jeśli checksum się nie zgadza
    if (checksum_ == checksumC) {
        return lastDataframe;
    } else {
        std::cout << "Checksum error" << '\n';
        return 0xFFFFFFFF;
    }
}