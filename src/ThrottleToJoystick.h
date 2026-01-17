#pragma once

#include <Arduino.h>
#include <Joystick_ESP32S2.h>

class ThrottleToJoystick {
public:
    ThrottleToJoystick();
    void begin();
    void update(uint8_t* spiData);

private:
    Joystick_ joystick;
};
