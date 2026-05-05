# Unity Game Controller (Hardware Firmware)

This project contains the firmware for a physical game controller based on a microcontroller (ESP32/ESP8266), developed using **PlatformIO**. The device is designed to send input signals to a Unity application via network or serial protocol[cite: 1].

## 🛠️ Tech Stack
*   **Framework:** Arduino / PlatformIO[cite: 1]
*   **Supported Hardware:** ESP32 / ESP8266[cite: 1]
*   **Language:** C++ (C# on the Unity side)[cite: 1]

## 📂 Project Structure
*   `src/main.cpp`: Main firmware logic and physical input management[cite: 1].
*   `include/credentials.h`: Wi-Fi credentials configuration (do not publish!)[cite: 1].
*   `platformio.ini`: Build environment and library configuration[cite: 1].
*   `lib/`: Local libraries specific to the controller[cite: 1].

## 🚀 Setup and Installation

### 1. Prerequisites
Ensure you have [Visual Studio Code](https://code.visualstudio.com/) installed with the **PlatformIO IDE** extension[cite: 1].

### 2. Credentials Management
The project uses a protection system for sensitive credentials[cite: 1]:
1. Rename the file `include/template_credentials.h` to `include/credentials.h`[cite: 1].
2. Enter your Wi-Fi SSID and Password inside the file[cite: 1].
   *(Note: `credentials.h` is already included in `.gitignore` for security purposes)*[cite: 1].

### 3. Build and Upload
1. Connect the microcontroller to your computer via USB[cite: 1].
2. Click the arrow icon (→) in the PlatformIO status bar to compile and upload the code to the board[cite: 1].

## 🕹️ How it Works
The controller reads data from sensors or buttons connected to the microcontroller pins and packages them to be received by Unity[cite: 1]. Make sure the controller's IP address and the PC running Unity are on the same subnet[cite: 1].

## 📝 License
Distributed under the MIT License. See the `LICENSE` file for details[cite: 1].