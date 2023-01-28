# Bluetooth Metronome Arm Band
Designing and Building a metronome arm band using an ESP32 module, controlled by smartphone using Bluetooth data connection.
Project includes FreeRTOS library for multitasking in order to maintain beats per minute accuracy.

## Setup
1. Download Arduino IDE, and Serial Bluetooth Terminal from playstore.
2. Install FreeRTOS library in the Arduino IDE if you haven't from the top menu.
3. In the Serial Bluetooth Terminal application, set the buttons below to the the following value:
   a. On       : 1
   b. Off      : 0
   c. Increase : p
   d. Decrease : n
   e. Reset    : r
4. Upload main.ino to ESP32 and run.

## Schematic for ESP32
