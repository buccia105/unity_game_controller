#include "Display.h"
#include "Dashboard.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// supplementary pins for the st7789
#define TFT_CS D7
#define TFT_DC D6
#define TFT_RST D3

// input smoothing variables for the wireframe
float smoothPitch = 0.0f;
float smoothRoll = 0.0f;
const float smoothingFactor = 0.2f;

// creating the display
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
unsigned long messageClearTime = 0;

// initialization function
void initDisplay() {
    tft.init(240, 320);
    tft.setRotation(1);
    tft.fillScreen(ST77XX_BLACK);
    
    tft.setTextColor(ST77XX_CYAN);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.print("ESP32 RACING");
    
    tft.drawFastHLine(0, 35, 240, ST77XX_BLUE);
    
    tft.setTextColor(ST77XX_YELLOW);
    tft.setCursor(10, 60);
    tft.print("Pitch: ");
    tft.setCursor(10, 100);
    tft.print("Roll : ");

    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(10, 150);
    tft.print("WiFi Connecting...");

    initDashboard();
}

// printing wifi status
void showWiFiStatus(bool connected) {
    tft.fillRect(0, 140, 240, 40, ST77XX_BLACK);
    if (connected) {
        tft.setCursor(10, 150);
        tft.setTextColor(ST77XX_GREEN);
        tft.print("WiFi OK!");
        delay(1000);
        tft.fillRect(0, 140, 240, 40, ST77XX_BLACK);
    }
}

// sensor errors
void showSensorError() {
    tft.fillRect(0, 140, 240, 40, ST77XX_BLACK);
    tft.setTextColor(ST77XX_RED);
    tft.setCursor(10, 150);
    tft.print("SENSOR ERROR!");
}

// calibration
void showCalibrationMessage(unsigned long currentMillis) {
    tft.fillRect(0, 180, 240, 40, ST77XX_BLACK);
    tft.setTextColor(ST77XX_GREEN);
    tft.setCursor(50, 190);
    tft.print("CALIBRATED!");
    messageClearTime = currentMillis + 2000;
}

// data showing
void updateDisplayValues(float targetPitch, float targetRoll) {
    
    // input smoothing using linear interpolation
    smoothPitch = (smoothPitch * (1.0f - smoothingFactor)) + (targetPitch * smoothingFactor);
    smoothRoll  = (smoothRoll  * (1.0f - smoothingFactor)) + (targetRoll  * smoothingFactor);

    // text formatting 
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setTextSize(3);

    tft.setCursor(100, 55);
    tft.printf("%+.2f ", smoothPitch);

    tft.setCursor(100, 95);
    tft.printf("%+.2f ", smoothRoll);

    // graphics update
    drawDashboard(&tft, smoothPitch, smoothRoll);
}

// clear old data
void handleDisplayTasks(unsigned long currentMillis) {
    if (messageClearTime > 0 && currentMillis > messageClearTime) {
        tft.fillRect(0, 180, 240, 40, ST77XX_BLACK);
        messageClearTime = 0;
    }
}