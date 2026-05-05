#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

void initDisplay();
/* @brief adaas
*/
void showWiFiStatus(bool connected);
void showSensorError();
void showCalibrationMessage(unsigned long currentMillis);
void updateDisplayValues(float pitch, float roll);
void handleDisplayTasks(unsigned long currentMillis);

#endif