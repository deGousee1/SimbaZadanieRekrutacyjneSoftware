#include "TelemetryBlackBox.h"
#include <iostream>
#include <bitset>

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


    std::cout << std::bitset<32>(lastDataframe) << " LastDataframe" << '\n';
    return lastDataframe;
}