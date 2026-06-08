# Unity Game Controller (Hardware Firmware)

This project contains the firmware for a physical game controller based on a microcontroller (ESP32/ESP8266), developed using **PlatformIO**. The device is designed to send input signals to a Unity application via network or serial protocol.

## Tech Stack
*   **Framework:** Arduino / PlatformIO 
*   **Supported Hardware:** ESP32 / ESP8266 
*   **Language:** C++ (C# on the Unity side) 

## Project Structure
*   `src/main.cpp`: Main firmware logic and physical input management.
*   `include/credentials.h`: Wi-Fi credentials configuration (do not publish!).
*   `platformio.ini`: Build environment and library configuration.
*   `lib/`: Local libraries specific to the controller.
*   `PCB/`: Files needed to create the pcb.
*   `SHOE/`: Files needed to print the shoe.

## Setup and Installation

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

## How it Works
The controller reads data from sensors or buttons connected to the microcontroller pins and packages them to be received by Unity. Make sure the controller's IP address and the PC running Unity are on the same subnet.

## Hardware:
### PCB:
In the `PCB/` directory there are all the files needed to send to the pcb manufacturer, the schematics will help you build the hardware to test the controller, everything is already set up, just connect everything and flash the ESP32.

* remember to solder 2 wires on `D0` and `D1` pins to be able to send a "reset" command via osc via pin `D1`, and calibrate the ADXL375 with the pin `D0`.

### SHOE:
The shoe has been modified in Blender and Fusion360, so the size is fixed to 43EU, changing the size on the slicer or other software will end up breaking the dimentions for the hardware. The files should be printed as follows:

* `SHOE_modified.stl`: needs to be printed in TPU95A, my suggetion is with support enabled, it will be a pain to remove them but the final result will be worth.
* `electronics_boxes.step`: should be printed with PLA/PETG/ABS, a solid material to avoid mechanically stressing the electronics, enable supports only where is needed, like on the overhangs for the connectors holes.

## Video tour (fully assembled)
[![Tour](/IMAGES/IMG_1093.JPG)](https://youtube.com/shorts/1ewApq3uuPU?feature=share)

## Video DEMO
[![DEMO](/IMAGES/IMG_1094.JPG)](https://youtube.com/shorts/eRb2sZXc62k?feature=share)



## 📝 License
Distributed under the MIT License. See the `LICENSE` file for details.