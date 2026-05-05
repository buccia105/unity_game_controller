#ifndef OSC_H
#define OSC_H

#include <Arduino.h>

// Returns true if setup was successful, false if sensor failed
bool initOSC(); 

// Processes sensor data every 20ms. Returns true if new data was sent this frame.
bool processOSC(unsigned long currentMillis, float &outPitch, float &outRoll, bool &calibratedJustNow);

#endif