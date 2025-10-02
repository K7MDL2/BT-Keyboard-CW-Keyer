CW keyer with BT keyboard on ESP32-WROOM-32 module

This merges 2 projects and adds several new feqtures

1. The BT Keyboad Class was derived largely intact from https://github.com/turgu1/bt-keyboard.   

2. "The Arduino Morse Keyer" project from K6HX (code location TBD) 

I am using a Rii model i8+ (aka K08) mini dual mode keyboard with touchpad and buttons.  The bt_keyboard code was looking for appearance == 0x031c and UUID == 0x1812 to initiate connection to the keyboard.  

The Rii keyboard showed up as UUID=0x0000.  I bypassed the UUID match and just use appearance value. The code scans BLE thgen BT to build a device list.  It then scans the list and connects to the first device that matches the appearance number.

As of 10/1 initial code check-in, the code accepts 0-9, a-z, and space character.  Letters are converted to upper case.  The characters are sent out real time as morse code on a PWM I/O pin.  I hgooked up a stanadrd piezo buzzer.  The speed and frequency are set in the code.  A (blue in my case) LED on pin 2 also lights.

Hardware is a ESP32-WROOM-32 module from https://www.amazon.com/dp/B0C5MJ6CPL.   Use care, some are older or even single core versions, many listing make it hard to tell.

Chip is ESP32-D0WD-V3 (revision v3.1)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
Crystal is 40MHz

I am using esp-idf in Visual Studio Code.  I had to add  the Arduino32 component to support the arduino Tone() and noTone() functions.  The bt_keyboard .cpp and .hpp are under components/bt_keyboard/src folder.  the CamkeLists.tx files are set accordingly.  I edited the c_cpp_properties.json and sdkconfig and sdkconfig.defaults quite a bit to get things working.
