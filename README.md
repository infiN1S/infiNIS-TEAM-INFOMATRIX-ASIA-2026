![INFOMATRIX CXEMA-2026](https://github.com/user-attachments/assets/710ee794-36ea-46c3-a255-23dc5e89beec)# infiNIS-TEAM-INFOMATRIX-ASIA-2026
infiNIS TEAM | INFOMATRIX-ASIA 2026 code


# infiNIS TEAM — INFOMATRIX ASIA 2026

Project Demonstration  
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

