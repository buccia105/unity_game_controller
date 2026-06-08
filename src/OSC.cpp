#include "OSC.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL375.h>

#include "credentials.h"
#include "esp32-hal-touch.h"

// osc settings IP and PORT can be defined in the credentials.h
const char* unityIP = UNITY_IP; 
const unsigned int unityPort = UNITY_PORT; 
WiFiUDP Udp;

// sensor implementation
Adafruit_ADXL375 accel = Adafruit_ADXL375(12345);
const float maxTiltAccel = 5.0f; 

// calibration variables
float offsetX = 0.0f;
float offsetY = 0.0f;

// touch button variables

const int calibrationTouchPin = D0; // calibration PIN
const int resetTouchPin = D1; // run reset PIN
const int touchThreshold = 30000;
const int resetThreashold = 40000;

// timing variables
unsigned long previousMillis = 0; 
const long interval = 20; 
unsigned long lastTouchTime = 0;
const long touchCooldown = 1000; 
unsigned int lastReset = 0;

// smoothing variables
float oscSmoothPitch = 0.0f;
float oscSmoothRoll = 0.0f;
const float oscSmoothingFactor = 0.25f;

// wi-fi connection
bool initOSC() {
    WiFi.begin(SECRET_WIFI_SSID, SECRET_WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    return accel.begin();
}


bool processOSC(unsigned long currentMillis, float &outPitch, float &outRoll, bool &calibratedJustNow, bool &resetJustNow) {
    calibratedJustNow = false;
    resetJustNow = false;

    int touchValue = touchRead(calibrationTouchPin);
    int resetValue = touchRead(resetTouchPin);
    Serial.printf("Calib(1): %d | Reset(D1): %d\n", touchValue, resetValue);

    // checks if touch value is over the threshold and for the cooldown to end
    if (touchValue > touchThreshold && (currentMillis - lastTouchTime > touchCooldown)) {
        
        // data fetching from sensor
        sensors_event_t event;
        accel.getEvent(&event);

        // update offset variables
        offsetX = event.acceleration.x;
        offsetY = event.acceleration.y;
        lastTouchTime = currentMillis; // updating timing variables (to avoid continuos calibration)
        calibratedJustNow = true;
    }

    // reset logic
    if (resetValue > touchThreshold && (currentMillis - lastReset > touchCooldown)) {
        lastReset = currentMillis;
        resetJustNow = true;

        OSCMessage msgReset("/car/reset");
        msgReset.add(1);
        Udp.beginPacket(unityIP, unityPort);
        msgReset.send(Udp);
        Udp.endPacket();
        msgReset.empty();
    }

    // sensor data trasmission every 20ms
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis; // update timing variables (emulates delay witout freezing the esp32)

        // data fetching from sensor
        sensors_event_t event;
        accel.getEvent(&event);

        // data calibration
        float calibratedX = event.acceleration.x - offsetX;
        float calibratedY = event.acceleration.y - offsetY;

        // mapping X Y to values between -1 and 1
        float rawGasBrake = calibratedX; 
        float rawSteering = -calibratedY;

        float targetPitch = constrain(rawGasBrake / maxTiltAccel, -1.0f, 1.0f);
        float targetRoll  = constrain(rawSteering / maxTiltAccel, -1.0f, 1.0f);

        oscSmoothPitch = (oscSmoothPitch * (1.0f - oscSmoothingFactor)) + (targetPitch * oscSmoothingFactor);
        oscSmoothRoll  = (oscSmoothRoll  * (1.0f - oscSmoothingFactor)) + (targetRoll  * oscSmoothingFactor);

        outPitch = oscSmoothPitch;
        outRoll  = oscSmoothRoll;

        // send osc steer
        OSCMessage msgSteer("/car/steer");
        msgSteer.add(outRoll);
        Udp.beginPacket(unityIP, unityPort);
        msgSteer.send(Udp);
        Udp.endPacket();
        msgSteer.empty();

        // send osc gas
        OSCMessage msgGas("/car/gas");
        msgGas.add(outPitch);
        Udp.beginPacket(unityIP, unityPort);
        msgGas.send(Udp);
        Udp.endPacket();
        msgGas.empty();

        return true; // data sent
    }

    return false; // if not enough time has passed yet
}