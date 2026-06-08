# Unity Game Controller (Hardware Firmware)

This project contains the firmware for a physical game controller based on a microcontroller (ESP32/ESP8266), developed using **PlatformIO**. The device is designed to send input signals to a Unity application via network or serial protocol.

## 🛠️ Tech Stack
*   **Framework:** Arduino / PlatformIO 
*   **Supported Hardware:** ESP32 / ESP8266 
*   **Language:** C++ (C# on the Unity side) 

## 📂 Project Structure
*   `src/main.cpp`: Main firmware logic and physical input management.
*   `include/credentials.h`: Wi-Fi credentials configuration (do not publish!).
*   `platformio.ini`: Build environment and library configuration.
*   `lib/`: Local libraries specific to the controller.

## 🚀 Setup and Installation

### 1. Prerequisites
Ensure you have [Visual Studio Code](https://code.visualstudio.com/) installed with the **PlatformIO IDE** extension.

### 2. Credentials Management
The project uses a protection system for sensitive credentials:
1. Rename the file `include/template_credentials.h` to `include/credentials.h`.
2. Enter your Wi-Fi SSID and Password inside the file.
   *(Note: `credentials.h` is already included in `.gitignore` for security purposes)*.

### 3. Build and Upload
1. Connect the microcontroller to your computer via USB.
2. Click the arrow icon (→) in the PlatformIO status bar to compile and upload the code to the board.

## 🕹️ How it Works
The controller reads data from sensors or buttons connected to the microcontroller pins and packages them to be received by Unity. Make sure the controller's IP address and the PC running Unity are on the same subnet.

## 📝 License
Distributed under the MIT License. See the `LICENSE` file for details.