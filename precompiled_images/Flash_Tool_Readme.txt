Flash Download Tool image files

This file set is tested on a ESP32-WROOM-32 clone board.

To flash firmware to your ESP32 board, follow the instructions located on the project GitHub site Wiki pages.

https://github.com/K7MDL2/BT-Keyboard-CW-Keyer/wiki/Using-the-Flash-Download-Tool


There are 3 files plus this readme file.  The filenames are listed below along with the offset number that you will enter with it in the Flash Download Tool UI.

1. Bootloader.bin   0x1000 (1 plus 3 zeros)
2. bt-keyboard-cw-keyer.bin  0x20000  (2 plus 4 zeros)
3. partition-table.bin 0x9000 (9 plus 3 zeros)
