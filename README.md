![INFOMATRIX CXEMA-2026](https://github.com/user-attachments/assets/710ee794-36ea-46c3-a255-23dc5e89beec)# infiNIS-TEAM-INFOMATRIX-ASIA-2026
infiNIS TEAM | INFOMATRIX-ASIA 2026 code


# infiNIS TEAM — INFOMATRIX ASIA 2026

Project Demonstration  
![WhatsApp Image 2026-02-14 at 14 16 07](https://github.com/user-attachments/assets/cbc7081d-4656-405d-8ff5-23f129a42665)

https://youtu.be/wcpWxVT-lmU?si=uNbnNw2dgn9xYBOi

---

## Overview

This repository contains the complete firmware and configuration for the infiNIS TEAM robot developed for INFOMATRIX ASIA 2026.

The system is built on ESP32 and includes:
- 4-motor drive system
- high-frequency PWM control (20 kHz)
- WiFi access point mode
- modular motor architecture
- configuration storage using LittleFS

The project focuses on stability, scalability and clean architecture.

---

## Hardware

- ESP32 Dev Module  
- 4 DC motors  
- Motor drivers (PWM controlled)  
- External power supply  
- Custom chassis  

Important: Motors must NEVER be powered directly from ESP32 pins.

---

## Software Requirements

## Required Software and Libraries

Before uploading the firmware, make sure the following software and libraries are installed.

### 1. Arduino IDE
Install the latest stable version of Arduino IDE.

### 2. ESP32 Board Package
1) Open Arduino IDE  
2) Go to File → Preferences  
3) In "Additional Boards Manager URLs" add:
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json  
4) Go to Tools → Board → Boards Manager  
5) Search for "ESP32"  
6) Install the latest version by Espressif Systems  
7) Select board: ESP32 Dev Module  

### 3. Required Libraries

Install the following libraries via:
Sketch → Include Library → Manage Libraries

- WiFi (included with ESP32 core)
- SettingsGyver
- GyverDBFile
- LittleFS (included with ESP32 core)
- Bluepad32

Search each library by name in Library Manager and install the latest stable version.

### 4. Required Includes in Firmware

The project uses the following libraries in the code:

#include <Arduino.h>
#include <WiFi.h>
#include <SettingsGyver.h>
#include <GyverDBFile.h>
#include <LittleFS.h>
#include <Bluepad32.h>

Make sure all libraries are successfully installed before compiling.


---

## Installing ESP32 Board

1. Open Arduino IDE  
2. Go to File → Preferences  
3. Add ESP32 package URL into "Additional Boards Manager URLs"  
4. Open Tools → Board → Boards Manager  
5. Search for "ESP32 by Espressif Systems"  
6. Install latest stable version  
7. Select Tools → Board → ESP32 Dev Module  

---

## Installation

Download the repository as ZIP from GitHub and extract it.

Open the .ino file inside Arduino IDE.

---

## Uploading Firmware

1. Connect ESP32 via USB  
2. Select Board: ESP32 Dev Module  
3. Select correct COM Port  
4. Click Upload  
5. Wait until flashing is complete  

If upload fails:
- Hold BOOT button while clicking Upload
- Release BOOT when flashing starts

---

## WiFi Mode

After successful upload, ESP32 creates its own WiFi Access Point.

SSID and password are defined inside the firmware.

Connect to the ESP32 network to control the robot.

---

## System Architecture

The firmware includes:

- Motor struct-based configuration  
- Independent PWM channels per motor  
- Inversion control per motor  
- 20 kHz PWM frequency  
- Flash storage using LittleFS  

This structure allows future extension without rewriting core logic.

---

## Safety Notes

- Use stable external power supply  
- Do not power motors from ESP32  
- Disconnect power before changing wiring  
- Use proper motor drivers rated for your current  


---

## Control Modes and Code Description

This project supports two control modes: WiFi (phone control) and Bluetooth (gamepad control).

---

### 1) Phone Control (WiFi)

Photo:  
![WhatsApp Image 2026-02-15 at 23 20 40](https://github.com/user-attachments/assets/ad65dc93-e9a6-4e02-814a-90b34132c75f)


Description:  
The code implements Wi-Fi control of the robot.
The ESP32 creates its own access point and launches a web page with virtual joysticks, allowing real-time control of the robot directly from a smartphone browser.

Firmware file:  
[InfiNIS_code_wifi_phone](InfiNIS_code_wifi_phone)

---

### 2) Gamepad Control (Bluetooth – Bluepad32)

Photo:
![WhatsApp Image 2026-02-15 at 23 24 19](https://github.com/user-attachments/assets/600934f1-f969-47ac-af32-cd0454d40ac0)


Description:  
The code implements Bluetooth control of the robot.
The ESP32 connects to a Bluetooth gamepad and uses the joystick inputs to control the robot’s movement in real time.

Firmware file:  
[InfiNIS_code_bluetooth_joystick.ino](InfiNIS_code_bluetooth_joystick.ino)

---

## Development Process

The robot was fully built by our team from scratch — from mechanical base to final firmware testing.

### 1. Chassis Manufacturing

The base platform was made from plywood.  
We measured, marked and cut all structural elements manually to match our robot dimensions.

![WhatsApp Image 2026-02-15 at 23 06 37](https://github.com/user-attachments/assets/a524c51e-f72a-466d-97de-55b366eb3e35)


All parts were carefully aligned to ensure correct motor placement and structural rigidity.

---

### 2. Electronics Assembly & Wiring

After assembling the frame, we installed:

- ESP32 controller
- Motor drivers
- Power distribution system
- Battery pack

All connections were soldered manually to ensure stable power supply and reliable signal transmission.

![WhatsApp Image 2026-02-15 at 23 06 38](https://github.com/user-attachments/assets/951e9f1b-3be7-4023-9b68-f80e9732239f)


We carefully organized cables to avoid interference and voltage drops.

---

### 3. Final Robot Integration

After mechanical and electronic integration, the system was tested in both control modes:

- Bluetooth (Gamepad via Bluepad32)
- WiFi (Phone control)

![WhatsApp Image 2026-02-15 at 23 06 38 (1)](https://github.com/user-attachments/assets/1ea876c8-97d2-43f5-9006-eae838814d6e)


Multiple test sessions were conducted to verify motor synchronization, responsiveness and stability.

---

## Competition Results

### INFOMATRIX (Previous Season)

Our team participated in INFOMATRIX last year and achieved:

3rd place

This result reflects:

- Strong engineering design
- Reliable hardware implementation
- Stable firmware architecture
- Effective teamwork under competition conditions

![WhatsApp Image 2026-02-15 at 23 09 34](https://github.com/user-attachments/assets/e4325ce1-4d43-4ec7-bb89-5cc4cae53ba5)

![WhatsApp Image 2026-02-15 at 23 09 17 (1)](https://github.com/user-attachments/assets/57028b99-c116-4c1d-a345-cfd881e05851)

---

## Media Coverage

Our team's achievements were covered by official media sources:

- Source: 7-su.kz  
  https://share.google/nVxQP4ZnoGfBamJ7k  

- Source: Nazarbayev Intellectual Schools  
  https://share.google/n9fkkaTrxCs6kUAf5  

This experience helped us improve our current robot platform and software structure for the 2026 season.
