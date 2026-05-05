#include <Arduino.h>
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

// esp32 touch pin definition
const int touchPin = 1; 
const int touchThreshold = 70000; // threashold to detect touch

// timing variables
unsigned long previousMillis = 0; 
const long interval = 20; 

// touch debounce
unsigned long lastTouchTime = 0;
const long touchCooldown = 1000; // wait 1sec between calibrations

void setup() {
  Serial.begin(115200);

  // wi-fi connection
  Serial.print("Connecting to WiFi");
  WiFi.begin(SECRET_WIFI_SSID, SECRET_WIFI_PASS); // ssid and password passed from credentials.h
  
  // debug print for wi-fi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  // ADXL375 accellerometer initializartion
  Serial.println("initializing ADXL375");
  if (!accel.begin()) {
    Serial.println("not detected");
    while (1);
  }
  Serial.println("ADXL375 found");
}

void loop() {
  unsigned long currentMillis = millis();
  // calibration logic
  // capacitive read from touch sensor
  int touchValue = touchRead(touchPin);

  // checks if touch value is over the threshold and for the cooldown to end
  if (touchValue > touchThreshold && (currentMillis - lastTouchTime > touchCooldown)) {
    Serial.println("--- CALIBRATION TRIGGERED ---");
    
    // sensor red to get an offset
    sensors_event_t event;
    accel.getEvent(&event);
    
    // saving current X, Y values for 0
    offsetX = event.acceleration.x;
    offsetY = event.acceleration.y;
    
    lastTouchTime = currentMillis; // reset debouce timer
    Serial.printf("new offset: X = %.2f | Y = %.2f\n", offsetX, offsetY);
  }

  // sensor transmission
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // reading raw data
    sensors_event_t event;
    accel.getEvent(&event);

    // subtract offset
    float calibratedX = event.acceleration.x - offsetX;
    float calibratedY = event.acceleration.y - offsetY;

    // accelleration map
    float rawGasBrake = calibratedX; 
    float rawSteering = - calibratedY; // the negative sign is to correct the orientation of the sensor

    // normalizing raw data and mapping it [-1,1]
    float pitch = constrain(rawGasBrake / maxTiltAccel, -1.0f, 1.0f);
    float roll  = constrain(rawSteering / maxTiltAccel, -1.0f, 1.0f);

    // osc send
    OSCMessage msgSteer("/car/steer");
    msgSteer.add(roll); //sending roll as steer
    Udp.beginPacket(unityIP, unityPort);
    msgSteer.send(Udp);
    Udp.endPacket();
    msgSteer.empty();

    OSCMessage msgGas("/car/gas");
    msgGas.add(pitch); // sending pitch as throttle
    Udp.beginPacket(unityIP, unityPort);
    msgGas.send(Udp);
    Udp.endPacket();
    msgGas.empty();

    // debug for calibration
    Serial.printf("Touch: %d | Pitch: %.2f | Roll: %.2f\n", touchValue, pitch, roll);
  }
}