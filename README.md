| Support Targets | ![alt text][esp32] |
| --- | --- |

| Boards Used | ![alt text][esp32-WROOM-32]|
| --- | --- |

[esp32]: https://img.shields.io/badge/ESP32-green "ESP32"
[esp32-WROOM-32]: https://img.shields.io/badge/ESP32--WROOM--32-orange "ESP32-WROOM-32"

CW keyer with BT keyboard on ESP32-WROOM-32 module   ( under construction )

This merges 2 projects   It is early days with this first effort to replace the PS2Keyboard with the BT keyboard and send out basic ketters and nuymbers to a buzzer and LED.  May add features like macros, memories, own callsign, own grid, destination call and grid and a screen.   Going to consider using this experience to add BT Keybopard to something like the K3NG keyer project.

1. The BT Keyboad Class was derived largely intact from https://github.com/turgu1/bt-keyboard.   

2. "The Arduino Morse Keyer" project from K6HX (code location TBD) 

I am using a Rii model i8+ (aka K08) mini dual mode keyboard with touchpad and buttons.  The bt_keyboard code was looking for appearance == 0x031c and UUID == 0x1812 to initiate connection to the keyboard.  

The Rii keyboard showed up as UUID=0x0000.  I bypassed the UUID match and just use appearance value. The code scans BLE thgen BT to build a device list.  It then scans the list and connects to the first device that matches the appearance number.

As of 10/1 initial code check-in, the code accepts 0-9, a-z, and space character.  Letters are converted to upper case.  The characters are sent out real time as morse code on a PWM I/O pin.  I hgooked up a stanadrd piezo buzzer.  The speed and frequency are set in the code.  A (blue in my case) LED on pin 2 also lights.

Hardware is a ESP32-WROOM-32 module from https://www.amazon.com/dp/B0C5MJ6CPL.   Use care, some are older or even single core versions, many listing make it hard to tell.

Chip is ESP32-D0WD-V3 (revision v3.1)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
Crystal is 40MHz

I am using esp-idf 5.5.1 in Visual Studio Code.  I had to add the Arduino32 component to support the arduino Tone() and noTone() functions rather then mek the effort to set up PWM channels (using ledc) manaully).  The bt_keyboard .cpp and .hpp are under components/bt_keyboard/src folder.  The CamkeLists.tx files are set accordingly.  I edited the c_cpp_properties.json and sdkconfig and sdkconfig.defaults quite a bit to get things working.


