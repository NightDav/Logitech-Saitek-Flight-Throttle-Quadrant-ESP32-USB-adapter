#pragma once

#include <Arduino.h>

// Pin Definitions for ESP32-S3 Super Mini
namespace Pins {
    constexpr int THROTTLE_DATA = 4;
    constexpr int THROTTLE_CLOCK = 5;
    constexpr int THROTTLE_READY = 6;  // RDY (INT0 equivalent)
    constexpr int LED_GREEN = 7;
    constexpr int LED_RED = 9;         // GPIO 9 (Changed from 8 to avoid conflict with onboard LED)
}

// Timing Constants for Throttle Protocol
namespace Timing {
    constexpr int HOLDOFF_FREQ_HZ = 525;
    //constexpr int READ_FREQ_HZ = 50000;
    constexpr int READ_FREQ_HZ = 20000;
    
    // Half-period in microseconds
    //constexpr int CLOCK_HALF_PERIOD_US = 10; // 50kHz -> 20us period -> 10us half
    constexpr int CLOCK_HALF_PERIOD_US = 25; // 20kHz -> 50us period -> 25us half
}
