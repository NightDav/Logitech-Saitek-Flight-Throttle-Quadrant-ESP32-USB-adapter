#include "ThrottleToJoystick.h"

// Define Joystick Config
// 9 Buttons, 3 Axes (X, Y, Z, no Rx/Ry/Rz/Rudder/Throttle/Accel/Brake/Steering)
// includeXAxis, includeYAxis, includeZAxis, includeRxAxis...
ThrottleToJoystick::ThrottleToJoystick() 
    : joystick(
        JOYSTICK_DEFAULT_REPORT_ID, 
        JOYSTICK_TYPE_JOYSTICK,
        9,    // Button Count
        0,    // Hat Switch Count
        true, // X
        true, // Y
        true, // Z
        false, false, false, false, false, false, false, false // Other axes
      ) 
{
    // Set ranges to 0-255 to match raw byte data
    joystick.setXAxisRange(0, 255);
    joystick.setYAxisRange(0, 255);
    joystick.setZAxisRange(0, 255);
}

void ThrottleToJoystick::begin() {
    joystick.begin();
}

void ThrottleToJoystick::update(uint8_t* spiData) {
    // Map Axes
    joystick.setXAxis(spiData[0]);
    joystick.setYAxis(spiData[1]);
    joystick.setZAxis(spiData[2]);

    // Map Buttons
    // Byte 3: Buttons 1-8
    for (int i = 0; i < 8; ++i) {
        bool pressed = (spiData[3] >> i) & 0x01;
        joystick.setButton(i, pressed ? 1 : 0);
    }

    // Byte 4: Button 9 (Bit 0)
    bool btn9 = (spiData[4] & 0x01);
    joystick.setButton(8, btn9 ? 1 : 0);
    
    // Send state
    // Note: The library sends updates automatically on set calls if autoSend is true (default)
    // or we can batch them. Default is true, so this is fine. 
    // Optimization: Disable autoSend in constructor and call sendState() here?
    // Let's assume default is okay for now, traffic is low (50Hz max probably).
}
