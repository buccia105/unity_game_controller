#include "OSC.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL375.h>

#include "credentials.h"

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
const int touchPin = 1; 
const int touchThreshold = 60000; 

// timing variables
unsigned long previousMillis = 0; 
const long interval = 20; 
unsigned long lastTouchTime = 0;
const long touchCooldown = 1000; 

// wi-fi connection
bool initOSC() {
    WiFi.begin(SECRET_WIFI_SSID, SECRET_WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    return accel.begin();
}


bool processOSC(unsigned long currentMillis, float &outPitch, float &outRoll, bool &calibratedJustNow) {
    calibratedJustNow = false;
    int touchValue = touchRead(touchPin);

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
        outPitch = constrain(rawGasBrake / maxTiltAccel, -1.0f, 1.0f);
        outRoll  = constrain(rawSteering / maxTiltAccel, -1.0f, 1.0f);

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