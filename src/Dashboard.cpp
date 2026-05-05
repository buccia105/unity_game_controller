#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <math.h>
#include "Dashboard.h"

// variables to avoid flickering
// roll variables
int prevX1, prevY1, prevX2, prevY2;
int rollCX = 160, rollCY = 180; // center of the line
int rollLen = 80;               // line length
// pitch variables
int barX = 30, barY = 140; // bottom right origin
int barW = 20, barH = 80;  // width and hight

void initDashboard() {
    // initializing horizon
    prevX1 = rollCX - rollLen / 2;
    prevY1 = rollCY;
    prevX2 = rollCX + rollLen / 2;
    prevY2 = rollCY;
}

void drawDashboard(Adafruit_GFX* tft, float pitch, float roll) {
    
    // pitch bar
    int halfH = barH / 2;
    int centerY = barY + halfH;
    
    // constraining pitch between +-1
    float safePitch = constrain(pitch, -1.0f, 1.0f);
    
    // vertical fill
    int fill_h = (int)(abs(safePitch) * halfH);
    
    // pitching forward
    if (safePitch > 0.01f) {
        if (halfH - fill_h > 0) {
            tft->fillRect(barX, barY, barW, halfH - fill_h, ST77XX_BLACK); 
        }
        // fillinf green from the center
        if (fill_h > 0) {
            tft->fillRect(barX, centerY - fill_h, barW, fill_h, ST77XX_GREEN);
        }
        // emptying the bar
        tft->fillRect(barX, centerY, barW, halfH, ST77XX_BLACK);
        
    } 

    // pitching backward
    else if (safePitch < -0.01f) {
        // clearing over the center
        tft->fillRect(barX, barY, barW, halfH, ST77XX_BLACK);
        // filling red from the center 
        if (fill_h > 0) {
            tft->fillRect(barX, centerY, barW, fill_h, ST77XX_RED);
        }
        // emptying the bar
        if (halfH - fill_h > 0) {
            tft->fillRect(barX, centerY + fill_h, barW, halfH - fill_h, ST77XX_BLACK);
        }
        
    }

    // neutral
    else {
        tft->fillRect(barX, barY, barW, barH, ST77XX_BLACK);
    }
    
    // drawing external borders
    tft->drawRect(barX - 1, barY - 1, barW + 2, barH + 2, ST77XX_WHITE);
    // yellow mid point
    tft->drawFastHLine(barX - 5, centerY, barW + 10, ST77XX_YELLOW);


    // horizon line
    tft->drawLine(prevX1, prevY1, prevX2, prevY2, ST77XX_BLACK);
    
    // +- 45°
    float angle = roll * (PI / 4.0); 
    
    int dx = (int)((rollLen / 2) * cos(angle));
    int dy = (int)((rollLen / 2) * sin(angle));
    
    int x1 = rollCX - dx;
    int y1 = rollCY - dy;
    int x2 = rollCX + dx;
    int y2 = rollCY + dy;
    
    // drawing new line
    tft->drawLine(x1, y1, x2, y2, ST77XX_CYAN);
    tft->fillCircle(rollCX, rollCY, 3, ST77XX_WHITE);
    
    prevX1 = x1; prevY1 = y1;
    prevX2 = x2; prevY2 = y2;
}