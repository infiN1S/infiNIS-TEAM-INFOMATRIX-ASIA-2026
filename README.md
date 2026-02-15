# infiNIS-TEAM-INFOMATRIX-ASIA-2026


![INFOMATRIX CXEMA-2026](https://github.com/user-attachments/assets/710ee794-36ea-46c3-a255-23dc5e89beec)

infiNIS team car scheme for participation in INFOMATRIX 2026 (Arduino Hackathon category)


Project Demonstration  
![WhatsApp Image 2026-02-14 at 14 16 07](https://github.com/user-attachments/assets/cbc7081d-4656-405d-8ff5-23f129a42665)

https://youtu.be/wcpWxVT-lmU?si=uNbnNw2dgn9xYBOi

---

## Navigation

- [Overview](#overview)
- [Development Environment and Implementation](#development-environment-and-implementation)
- [Control Modes and Code Description](#control-modes-and-code-description)
  - [Phone Control (WiFi)](#1-phone-control-wifi)
  - [Gamepad Control (Bluetooth – Bluepad32)](#2-gamepad-control-bluetooth--bluepad32)
- [Development Process](#development-process)
  - [Chassis Manufacturing](#1-chassis-manufacturing)
  - [Electronics Assembly & Wiring](#2-electronics-assembly--wiring)
  - [Final Robot Integration](#3-final-robot-integration)
- [Competition Results](#competition-results)
- [Media Coverage](#media-coverage)

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

## Development Environment and Implementation

The robot firmware was developed using Arduino IDE with ESP32 board support installed.

The project is built on the ESP32 Dev Module platform.  
ESP32 board package by Espressif Systems was installed through Arduino Board Manager to enable compilation and flashing.

The following libraries were used in the firmware:

- WiFi (built-in ESP32 library)
- LittleFS (internal flash file system)
- SettingsGyver
- GyverDBFile
- Bluepad32 (for Bluetooth gamepad control)

WiFi and LittleFS are included within the ESP32 board core package.  
SettingsGyver and GyverDBFile are used for structured configuration handling and persistent parameter storage.  
Bluepad32 provides real-time Bluetooth gamepad input processing.

The firmware was compiled and uploaded to the ESP32 Dev Module using USB connection.  
Standard flash configuration settings were used for stable operation.

After flashing, the ESP32 operates in Access Point mode for WiFi control and supports Bluetooth pairing for gamepad control.

This development setup ensures reliable firmware updates, modular expansion and repeatable deployment across hardware revisions.


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
