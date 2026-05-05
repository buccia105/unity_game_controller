#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <Adafruit_GFX.h>

// Inizializza le variabili per il cruscotto
void initDashboard();

// Disegna la barra e la linea di sterzo
void drawDashboard(Adafruit_GFX* tft, float pitch, float roll);

#endif