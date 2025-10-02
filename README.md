| Support Targets | ![alt text][esp32] |
| --- | --- |

| Boards Used | ![alt text][esp32-WROOM-32]|
| --- | --- |

[esp32]: https://img.shields.io/badge/ESP32-green "ESP32"
[esp32-WROOM-32]: https://img.shields.io/badge/ESP32--WROOM--32-orange "ESP32-WROOM-32"

CW keyer with BT keyboard on ESP32-WROOM-32 module   ( under construction )

As of Oct 2, 2025 I have CMake automatically copy binary image files to the "precompiled_images" folder.  You can now upload these image files to the CPU and skip compiling this code.  Follow the instructions on this Wiki page.
  https://github.com/K7MDL2/BT-Keyboard-CW-Keyer/wiki/Using-the-Flash-Download-Tool
Ignore any reference to ESP32-S3, the page was copied from one of my other projects, should be the same, just use the 3 files in the precompiled_images folder here.


This merges 2 projects.

1. The BT Keyboad Class was derived largely intact from https://github.com/turgu1/bt-keyboard.   

2. "The Arduino Morse Keyer" project from K6HX (code location TBD) 

It is early days with this first effort to replace the PS2Keyboard in "The Morse Keyer" by K6HX with the BT keyboard and send out basic letters and numbers to a buzzer and LED.  May add features like macros, memories, own callsign, own grid, destination call and grid and a screen.   Going to consider using this experience to add BT Keyboard to something like the K3NG keyer project.

I am using a Rii model i8+ (aka K08) mini dual mode keyboard with touchpad and buttons.  The bt_keyboard code was looking for appearance == 0x031c and UUID == 0x1812 to initiate connection to the keyboard.  The Rii keyboard showed up as UUID=0x0000.  I bypassed the UUID match and just use appearance value. The code scans BLE thgen BT to build a device list.  It then scans the list and connects to the first device that matches the appearance number.

As of 10/1/2025 initial code check-in, the code accepts 0-9, a-z, and space character.  Letters are converted to upper case.  The characters are sent out real time as morse code on a PWM I/O pin.  I hooked up a standard piezo buzzer.  The speed and frequency are set in the code.  An onboard user LED (blue in my case) on pin 2 also lights.

Hardware is a ESP32-WROOM-32 module from https://www.amazon.com/dp/B0C5MJ6CPL.   Use care, some are older or even single core versions, many listing make it hard to tell.

Chip is ESP32-D0WD-V3 (revision v3.1)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
Crystal is 40MHz

I am using esp-idf 5.5.1 in Visual Studio Code.  I had to add the Arduino32 component to support the arduino Tone() and noTone() functions rather then mek the effort to set up PWM channels (using ledc) manaully).  The bt_keyboard .cpp and .hpp are under components/bt_keyboard/src folder.  The CMakeLists.tx files are set accordingly.  I edited the c_cpp_properties.json and sdkconfig and sdkconfig.defaults quite a bit to get things working.







