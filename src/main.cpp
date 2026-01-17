#include <Arduino.h>
#include "Config.h"
#include "ThrottleReader.h"
#include "ThrottleToJoystick.h"

ThrottleReader throttleReader;
ThrottleToJoystick throttleJoystick;

void setup() {
    //Serial.begin(115200);
    //Serial.println("Starting DIY Saitek Throttle...");

	USB.PID(0x8211);
	USB.VID(0x303b);
	USB.productName("Pro Flight Quadrant");
	USB.manufacturerName("DIY");
	USB.begin();

    throttleReader.begin();
    throttleJoystick.begin();
}

void loop() {
    throttleReader.update();

    if (throttleReader.isDataFresh()) {
        uint8_t data[5];
        throttleReader.getData(data);
        
        throttleJoystick.update(data);
    }
}
