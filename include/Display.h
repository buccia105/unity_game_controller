#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

// These are the functions main.cpp is allowed to use
void initDisplay();
void showWiFiStatus(bool connected);
void showSensorError();
void showCalibrationMessage(unsigned long currentMillis);
void updateDisplayValues(float pitch, float roll);
void handleDisplayTasks(unsigned long currentMillis);

#endif