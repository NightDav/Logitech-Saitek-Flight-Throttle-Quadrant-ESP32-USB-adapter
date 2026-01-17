#include "ThrottleReader.h"

volatile bool ThrottleReader::readyTriggered = false;

void ThrottleReader::begin() {
    pinMode(Pins::THROTTLE_DATA, INPUT);
    pinMode(Pins::THROTTLE_CLOCK, OUTPUT);
    pinMode(Pins::THROTTLE_READY, INPUT_PULLUP); // Assuming Pullup as per typical active-low logic
    pinMode(Pins::LED_GREEN, OUTPUT);
    pinMode(Pins::LED_RED, OUTPUT);

    // Initial State
    digitalWrite(Pins::THROTTLE_CLOCK, HIGH); // Idle High? Or Low? Old code: set_clock_high during read loop, implies default might be high or don't care. Let's assume standard SPI mode.
    // Looking at old code: board_throttle_set_clock_low() then high. 
    
    digitalWrite(Pins::LED_GREEN, LOW);
    digitalWrite(Pins::LED_RED, HIGH); // Disconnected state (Red On)

    attachInterrupt(digitalPinToInterrupt(Pins::THROTTLE_READY), ThrottleReader::onReadyInterrupt, FALLING);
}

void IRAM_ATTR ThrottleReader::onReadyInterrupt() {
    readyTriggered = true;
}

void ThrottleReader::update() {
    if (readyTriggered) {
        readyTriggered = false;

        // Debounce / Holdoff
        // Old code had "HOLDOFF" state ~1.9ms.
        // We can just delay here since we are in the main loop (update called from loop)
        // or strictly speaking, if we want to be non-blocking, we should check time.
        // BUT, for simplicity and since the read is blocking anyway, let's just do the sequence.
        
        if (!connected) {
            connected = true;
            digitalWrite(Pins::LED_GREEN, HIGH);
            digitalWrite(Pins::LED_RED, LOW);
        }

        delayMicroseconds(1900); // 1.9ms Holdoff

        readData();
    }
}

void ThrottleReader::readData() {
    // Disable interrupts during critical timing? 
    // Maybe not strictly necessary on ESP32 if we use simple delays, but good practice.
    // portDISABLE_INTERRUPTS(); 

    for (int i = 0; i < 5; ++i) {
        uint8_t byteVal = 0;
        for (int j = 0; j < 8; ++j) {
            // Clock Low
            digitalWrite(Pins::THROTTLE_CLOCK, LOW);
            delayMicroseconds(Timing::CLOCK_HALF_PERIOD_US);

            // Clock High
            digitalWrite(Pins::THROTTLE_CLOCK, HIGH);
            
            // Read Bit (Sample on Rising Edge or shortly after?)
            // Old code: set high, read bit, wait flag.
            int bit = digitalRead(Pins::THROTTLE_DATA);
            byteVal |= (bit << j); // Old code: (board_throttle_read_bit() << j) -> LSB First

            delayMicroseconds(Timing::CLOCK_HALF_PERIOD_US);
        }
        dataBuffer[i] = byteVal;
    }

    // portENABLE_INTERRUPTS();
    newDataAvailable = true;
}

bool ThrottleReader::isDataFresh() {
    return newDataAvailable;
}

void ThrottleReader::getData(uint8_t* buffer) {
    if (newDataAvailable) {
        memcpy(buffer, dataBuffer, 5);
        newDataAvailable = false;
    }
}
