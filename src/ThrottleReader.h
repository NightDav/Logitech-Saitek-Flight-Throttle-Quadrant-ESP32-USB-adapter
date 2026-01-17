#pragma once

#include <Arduino.h>
#include "Config.h"

class ThrottleReader {
public:
    void begin();
    void update();
    bool isDataFresh();
    void getData(uint8_t* buffer);

private:
    static void IRAM_ATTR onReadyInterrupt();
    void readData();

    static volatile bool readyTriggered;
    bool connected = false;
    unsigned long lastReadTime = 0;
    
    uint8_t dataBuffer[5] = {0};
    bool newDataAvailable = false;
};
