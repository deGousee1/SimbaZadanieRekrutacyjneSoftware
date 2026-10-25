#include "TelemetryBlackBox.h"
#include <iostream>
#include <bitset>
#include <string>

void TelemetryBlackBox::onRocketStateChange(RocketState state) {
    state_ = state;
}

void TelemetryBlackBox::onBaroReading(bool error, uint16_t value) {
    baroError = error;
    altitude = value;
}

void TelemetryBlackBox::onParachuteDeployed() {
    parachute = true;
}

void TelemetryBlackBox::recordCurrentState() {
    if (state_ == RocketState::DISARM or state_ == RocketState::LANDED) {
        altitude = 0;
    }
    if (baroError == true) {
        altitude = 0b1111111111111111;
    }
    byte0 = (state_ & 0b111) | (parachute << 3) | (baroError << 4);
    byte1 = altitude & 0b11111111;
    byte2 = (altitude >> 8) & 0b11111111;
    checksum = byte0 ^ byte1 ^ byte2;
    frame = (checksum << 24) | (byte2 << 16) | (byte1 << 8) | byte0;

    if (pointer == 16) {
        pointer = 0;
    }
    buffer[pointer] = frame;
    pointer++;

    std::cout << std::bitset<32>(frame) << " CurrentState" << '\n';

}

uint32_t TelemetryBlackBox::getLastDataframe() {
    if (pointer == 0) {
        return 0xFFFFFFFF;
    }
    lastDataframe = buffer[pointer-1];

    uint8_t byte0_ = lastDataframe & 0b11111111;
    uint8_t byte1_ = (lastDataframe >> 8) & 0b11111111;
    uint8_t byte2_ = (lastDataframe >> 16) & 0b11111111;
    uint8_t checksumC = lastDataframe >> 24;
    uint8_t checksum_ = byte0_ ^ byte1_ ^ byte2_;

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

    uint8_t parachuteCout = (lastDataframe >> 3) & 0b1;
    std::string parachuteFinal;
    if (parachuteCout == 0) {
        parachuteFinal = "CLOSED";
    } else {
        parachuteFinal = "DEPLOYED";
    }

    uint8_t baroErrorCout = (lastDataframe >> 4) & 0b1;
    std::string baroErrorFinal;
    if (baroErrorCout == 0) {
        baroErrorFinal = "VALID";
    } else {
        baroErrorFinal = "ERROR";
    }

    uint8_t reservedCout = (lastDataframe >> 5) & 0b111;
    uint16_t altitudeCout = (lastDataframe >> 8) & 0b1111111111111111;

    std::cout << "State: " << stateFinal << "| Parachute: " << parachuteFinal << "| Baro: " << baroErrorFinal << "| Reserved: " << std::bitset<3>(reservedCout) << "| Altitude: " << altitudeCout << " feet" << " Checksum: " << std::bitset<8>(checksumC) << std::endl;

    if (checksum_ == checksumC) {
        return lastDataframe;
    } else {
        std::cout << "Checksum error" << '\n';
        return 0xFFFFFFFF;
    }
}