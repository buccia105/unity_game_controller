#include <Arduino.h>
#include "Display.h"
#include "OSC.h"

void setup() {
    Serial.begin(115200);
    
    // starting the screen
    initDisplay();

    // starting wi-fi and getting a response
    if (!initOSC()) {
        showSensorError();
        while(1); 
    }

    // showing wi-fi status
    showWiFiStatus(true);
}

void loop() {

    // timing variable
    unsigned long currentMillis = millis();

    // variables coming from osc functions
    float pitch = 0.0f;
    float roll = 0.0f;
    bool justCalibrated = false;

    // running osc and sensor logic
    // returns true if enough time has passed and we are ready to fetch data
    if (processOSC(currentMillis, pitch, roll, justCalibrated)) {
        // redraw new values if there is new data
        updateDisplayValues(pitch, roll);
    }

    // showing if the accellerometer was just calibrated
    if (justCalibrated) {
        showCalibrationMessage(currentMillis);
    }

    // display cleanup
    handleDisplayTasks(currentMillis);
}