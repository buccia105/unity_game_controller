#ifndef WIREFRAME3D_H
#define WIREFRAME3D_H

#include <Adafruit_GFX.h>

// Inizializza il centro dello schermo per il 3D
void init3D(int centerX, int centerY);

// Disegna il cubo ruotato in base a pitch e roll
void drawWireframe(Adafruit_GFX* tft, float pitch, float roll);

#endif