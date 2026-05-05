# Unity Game Controller (Hardware Firmware)

Questo progetto contiene il firmware per un controller fisico basato su microcontrollore (ESP32/ESP8266), sviluppato con **PlatformIO**. Il dispositivo è progettato per inviare segnali di input a un'applicazione Unity tramite rete o protocollo seriale.

## 🛠️ Stack Tecnologico
*   **Framework:** Arduino / PlatformIO
*   **Hardware supportato:** ESP32 / ESP8266
*   **Linguaggio:** C++ (C# lato Unity)

## 📂 Struttura del Progetto
*   `src/main.cpp`: Logica principale del firmware e gestione degli input fisici.
*   `include/credentials.h`: Configurazione delle credenziali Wi-Fi (da non pubblicare!).
*   `platformio.ini`: Configurazione dell'ambiente di build e delle librerie.
*   `lib/`: Librerie locali specifiche per il controller.

## 🚀 Configurazione e Installazione

### 1. Prerequisiti
Assicurati di avere installato [Visual Studio Code](https://code.visualstudio.com/) con l'estensione **PlatformIO IDE**.

### 2. Gestione Credenziali
Il progetto utilizza un sistema di protezione per le credenziali. 
1. Rinominare il file `include/template_credentials.h` in `include/credentials.h`.
2. Inserire il proprio SSID e la Password del Wi-Fi all'interno del file.
   *(Nota: `credentials.h` è già inserito nel `.gitignore` per sicurezza)*.

### 3. Build e Upload
1. Collega il microcontrollore al computer tramite USB.
2. Clicca sull'icona della freccia (→) nella barra di stato di PlatformIO per compilare e caricare il codice sulla scheda.

## 🕹️ Funzionamento
Il controller legge i dati dai sensori/pulsanti collegati ai pin del microcontrollore e li impacchetta per essere ricevuti da Unity. Assicurati che l'indirizzo IP del controller e quello del PC dove gira Unity siano sulla stessa sottorete.

## 📝 Licenza
Distribuito sotto licenza MIT. Vedere il file `LICENSE` per dettagli.