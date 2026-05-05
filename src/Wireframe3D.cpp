#include <math.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include "Wireframe3D.h"

// --- DIMENSIONI DEL CHIP (Parallelepipedo) ---
const float W = 60.0; // Larghezza
const float H = 15.0; // Spessore
const float D = 80.0; // Lunghezza

// Gli 8 angoli del chip in 3D (X, Y, Z)
const float vertices[8][3] = {
    {-W/2, -H/2, -D/2}, {W/2, -H/2, -D/2}, {W/2, H/2, -D/2}, {-W/2, H/2, -D/2},
    {-W/2, -H/2,  D/2}, {W/2, -H/2,  D/2}, {W/2, H/2,  D/2}, {-W/2, H/2,  D/2}
};

// Quali angoli collegare per formare le 12 linee del cubo
const int edgeList[12][2] = {
    {0,1}, {1,2}, {2,3}, {3,0}, // Faccia Posteriore
    {4,5}, {5,6}, {6,7}, {7,4}, // Faccia Anteriore
    {0,4}, {1,5}, {2,6}, {3,7}  // Linee di collegamento laterali
};

// Memoria per cancellare il frame precedente senza far sfarfallare lo schermo
int prevProj[8][2]; 
int cx = 120, cy = 160; // Coordinate del centro del cubo sullo schermo

void init3D(int centerX, int centerY) {
    cx = centerX;
    cy = centerY;
    // Inizializza tutto al centro
    for(int i=0; i<8; i++) { 
        prevProj[i][0] = cx; 
        prevProj[i][1] = cy; 
    }
}

void drawWireframe(Adafruit_GFX* tft, float pitch, float roll) {
    // 1. CANCELLA IL VECCHIO CUBO (Disegnandolo di nero)
    for (int i = 0; i < 12; i++) {
        int p1 = edgeList[i][0];
        int p2 = edgeList[i][1];
        tft->drawLine(prevProj[p1][0], prevProj[p1][1], prevProj[p2][0], prevProj[p2][1], ST77XX_BLACK);
    }
    // Cancella il vecchio pallino rosso
    tft->fillCircle(prevProj[4][0], prevProj[4][1], 4, ST77XX_BLACK);

    // 2. CONVERTI I VALORI (-1.0 a 1.0) IN RADIANTI
    // Moltiplichiamo per PI/2 (90 gradi) per avere una rotazione visiva realistica
    float pitchRad = pitch * (PI / 2.0); 
    float rollRad  = roll  * (PI / 2.0);

    float sinP = sin(pitchRad), cosP = cos(pitchRad);
    float sinR = sin(rollRad),  cosR = cos(rollRad);

    int currentProj[8][2];

    // 3. CALCOLA LA ROTAZIONE PER OGNI ANGOLO
    for (int i = 0; i < 8; i++) {
        float x = vertices[i][0];
        float y = vertices[i][1];
        float z = vertices[i][2];

        // Rotazione Asse X (Pitch)
        float xy = y * cosP - z * sinP;
        float xz = y * sinP + z * cosP;
        y = xy;
        z = xz;

        // Rotazione Asse Y (Roll)
        float yx = x * cosR + z * sinR;
        float yz = -x * sinR + z * cosR;
        x = yx;
        z = yz;

        // Proiezione Prospettica (Dà l'effetto 3D di profondità)
        float cameraZ = 200.0;
        float scale = cameraZ / (cameraZ + z);
        
        currentProj[i][0] = cx + (int)(x * scale);
        currentProj[i][1] = cy + (int)(y * scale);

        // Salva per cancellare al prossimo giro
        prevProj[i][0] = currentProj[i][0];
        prevProj[i][1] = currentProj[i][1];
    }

    // 4. DISEGNA IL NUOVO CUBO IN CIANO
    for (int i = 0; i < 12; i++) {
        int p1 = edgeList[i][0];
        int p2 = edgeList[i][1];
        tft->drawLine(currentProj[p1][0], currentProj[p1][1], currentProj[p2][0], currentProj[p2][1], ST77XX_CYAN);
    }

    // 5. DISEGNA IL PALLINO DI RIFERIMENTO SULL'ANGOLO IN ALTO A SINISTRA AVANTI
    tft->fillCircle(currentProj[4][0], currentProj[4][1], 4, ST77XX_RED);
}