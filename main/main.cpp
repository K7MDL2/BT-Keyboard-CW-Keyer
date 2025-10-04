// bt_keyboard
// Copyright (c) 2020 Guy Turcotte
// MIT License. Look at file licenses.txt for details.

//  Merged into 

//  _____ _            _          _      _            __  __                 
// |_   _| |_  ___    /_\  _ _ __| |_  _(_)_ _  ___  |  \/  |___ _ _ ___ ___ 
//   | | | ' \/ -_)  / _ \| '_/ _` | || | | ' \/ _ \ | |\/| / _ \ '_(_-</ -_)
//   |_| |_||_\___| /_/ \_\_| \__,_|\_,_|_|_||_\___/ |_|  |_\___/_| /__/\___|
//                                                                           
//  _  __                 
// | |/ /___ _  _ ___ _ _ 
// | ' </ -_) || / -_) '_|
// |_|\_\___|\_, \___|_|  
//           |__/   
//
// Version for the PS2 Keyboard - Modified to use BT keyboard
// using the code https://github.com/turgu1/bt-keyboard and converted toa library
// using the library from http://www.pjrc.com/teensy/td_libs_PS2Keyboard.html
// 
// Written by Mark VandeWettering K6HX
//
// This is just a quick Morse keyer program.
//

// Replaced PS2Keyboard with BT_Keyboard by K7MDL 10/2025

#include "Arduino.h"
#include <iostream>
#include "bt_keyboard.hpp"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"

#ifdef ARDUINO_ARCH_ESP32
#include "esp32-hal-log.h"
#endif

static constexpr char const *TAG = "Main";
char ch;
char ch_digit;
BTKeyboard bt_keyboard;
////////////////////////////////////////////////////////////////////////
int pin = 2 ;                  // blink the LED for now... 
int tpin = 25;                 // tone pin
int spin = 26;                 // speed sense switch
//pinMode(7, INPUT_PULLUP);
//if (digitalRead(spin) == LOW) wpm = 12; 
unsigned int freq = 700;
unsigned int wpm = 13;
int ditlen = 1200 / wpm ;

void pairing_handler(uint32_t pid) {
  std::cout << "Please enter the following pairing code, " << std::endl
            << "followed with ENTER on your keyboard: " << std::dec << pid << std::endl;
}

void keyboard_lost_connection_handler() {
  ESP_LOGW(TAG, "====> Lost connection with keyboard <====");
}

void keyboard_connected_handler() { ESP_LOGI(TAG, "----> Connected to keyboard <----"); }

////////////////////////////////////////////////////////////////////////
//
// Here is a queue to store the characters that I've typed.
// To simplify the code, it can store a maximum of QUEUESIZE-1 characters
// before it fills up.  What is a byte wasted between friends?
//
////////////////////////////////////////////////////////////////////////

// QUEUESIZE must be a power of two 
#define QUEUESIZE       (128)
#define QUEUEMASK       (QUEUESIZE-1)
#define DEBUG false
   
int aborted = 0 ;
int qhead = 0 ;
int qtail = 0 ;
char queue[QUEUESIZE] ;

void
queueadd(const char ch)
{
    queue[qtail++] = ch ;
    qtail &= QUEUEMASK ;
}
 
void
queueadd(const char *s)
{
  while (*s)
      queueadd(*s++) ;
}
 
char
queuepop()
{
    char ch ;
    ch = queue[qhead++] ;
    qhead &= QUEUEMASK ;
    return ch ;
}
 
int
queuefull()
{
    return (((qtail+1)%QUEUEMASK) == qhead) ;
}
 
int
queueempty()
{
    return (qhead == qtail) ;
}
 
void
queueflush()
{
    qhead = qtail ;
}

#ifdef PS2
  PS2Keyboard kbd ;
  
  inline void
  ps2poll()
  {
      
      char ch ;
      while (kbd.available()) {
          if (queuefull()) {
              Serial.print("") ;
          } else {
              switch (ch=kbd.read()) {
              case PS2_ENTER:
                  break ;
              case PS2_TAB:
                  break ;
              case PS2_PAGEDOWN:
                  break ;
              case PS2_PAGEUP:
                  break ;
              case PS2_LEFTARROW:
                  if (freq < 300) break;
                  freq -= 50;
  #if DEBUG 
                  Serial.print(F("FREQUENCY = ")) ;
                  Serial.println(freq) ;
  #endif                
                  break ;
              case PS2_RIGHTARROW:
                  if (freq > 2000) break;
                  freq += 50; 
  #if DEBUG 
                  Serial.print(F("FREQUENCY = ")) ;
                  Serial.println(freq) ;                
  #endif                
                  break ;
              case PS2_UPARROW:
                  if (wpm < 5) break;
                  wpm -= 1; 
                  ditlen = 1200 / wpm ;
  #if DEBUG 
                  Serial.print(F("WPM = ")) ;
                  Serial.println(wpm) ;                
  #endif                
                  break ;
              case PS2_DOWNARROW:
                  if (wpm > 30) break;
                  wpm += 1; 
                  ditlen = 1200 / wpm ;
  #if DEBUG 
                  Serial.print(F("WPM = ")) ;
                  Serial.println(wpm) ;                
  #endif                
                  break ;
              case PS2_DELETE:
                  break ;
              case PS2_ESC:
                  queueflush() ;
                  Serial.flush() ;
                  Serial.println(F("== FLUSH ==")) ;
                  aborted = 1 ;
                  break ;
              case '!':
                  queueadd("CQ CQ CQ DE W7RNB W7RNB BK\r\n") ;
                  break ;
              case '@':
                  queueadd("UR RST IS BK\r\n") ;
                  break;   
              case '#':
                  queueadd("RRR 73 RRR 73 TU  SK E E\r\n") ;
                  break;
              case '^' :
                  queueadd("CQ FD CQ FD DE W7RNB W7RNB BK \r\n") ;
                  break;
              case '&' :
                  queueadd("DE W7RNB 1D 1D WWA WWA BK\r\n") ;
                  break; 
              case '*' : 
                  queueadd("AGN? AGN?") ;
                  break;
              case '(' :
                  queueadd("QRZ QRZ DE W7RNB K\r\n") ;  
                  break;    
              default:
                  queueadd(ch) ;
                  break ;
              }
          }
      }
  }
#endif

void mydelay(unsigned long ms)
{
    //unsigned long t = millis() ;

    vTaskDelay(portTICK_PERIOD_MS * ms);

    //while (millis()-t < ms)
    //#ifdef PS2
    //    ps2poll() ;
    //#else
    //  ;
    //#endif
}
  
void scale()
{
  long f = 220L ;
  int i ;
   
  for (i=0; i<=12; i++) {
      tone(tpin, (int)f) ;
      f *= 1059L ;
      f /= 1000L ;
      Serial.println(f) ;
      delay(300) ;
  }
  noTone(tpin) ;
       
}
 
void dit()
{
    digitalWrite(pin, HIGH) ;
    tone(tpin, freq) ;
    mydelay(ditlen) ;
    digitalWrite(pin, LOW) ;
    noTone(tpin) ;
    mydelay(ditlen) ;
 
}
 
void dah()
{
    digitalWrite(pin, HIGH) ;
    tone(tpin, freq) ;
    mydelay(3*ditlen) ;
    digitalWrite(pin, LOW) ;
    noTone(tpin) ;
    mydelay(ditlen) ;
}
 
void lspace()
{
    mydelay(2*ditlen) ;
}
 
void space()
{
    mydelay(4*ditlen) ;
}
  
const char ltab[] = {
    0b101,              // A
    0b11000,            // B 
    0b11010,            // C
    0b1100,             // D
    0b10,               // E
    0b10010,            // F
    0b1110,             // G
    0b10000,            // H
    0b100,              // I
    0b10111,            // J
    0b1101,             // K
    0b10100,            // L
    0b111,              // M
    0b110,              // N
    0b1111,             // O
    0b10110,            // P
    0b11101,            // Q
    0b1010,             // R
    0b1000,             // S
    0b11,               // T
    0b1001,             // U
    0b10001,            // V
    0b1011,             // W
    0b11001,            // X
    0b11011,            // Y
    0b11100             // Z
} ;
 
const char ntab[] = {
    0b111111,           // 0
    0b101111,           // 1
    0b100111,           // 2
    0b100011,           // 3
    0b100001,           // 4
    0b100000,           // 5
    0b110000,           // 6
    0b111000,           // 7
    0b111100,           // 8
    0b111110            // 9
} ;
 
void sendcode(char code)
{
    int i ;
 
    for (i=7; i>= 0; i--)
        if (code & (1 << i))
            break ;
 
    for (i--; i>= 0; i--) {
        if (code & (1 << i))
            dah() ;
        else
            dit() ;
    }
    lspace() ;
}
 
void send(char ch)
{
    if (isalpha(ch)) {
        if (islower(ch)) ch = toupper(ch) ;
        sendcode(ltab[ch-'A']) ;
    } else if (isdigit(ch))
        sendcode(ntab[ch-'0']) ;
    else if (ch == ' ' || ch == '\r' || ch == '\n')
        space() ;
    else if (ch == '.')
        sendcode(0b1010101) ;
    else if (ch == ',')
        sendcode(0b1110011) ;
    else if (ch == '!')
        sendcode(0b1101011) ;
    else if (ch == '?')
        sendcode(0b1001100) ;
    else if (ch == '/')
        sendcode(0b110010) ;
    else if (ch == '+')
        sendcode(0b101010) ;
    else if (ch == '-')
        sendcode(0b1100001) ;
    else if (ch == '=')
        sendcode(0b110001) ;
    else if (ch == '@')         // hardly anyone knows this!
        sendcode(0b1011010) ;
    else
        return ;                // ignore anything else
 
    if (!aborted) {
      Serial.print(ch) ;
      if (ch == 13) Serial.print((char) 10) ;
    }
    aborted = 0 ;
}

//void setup() {}

//void loop() {
//  Serial.println("loop");
//}

extern "C" 
{ 
    void app_main() 
    {
        esp_err_t ret;

        pinMode(pin, OUTPUT) ;
        pinMode(tpin,   OUTPUT) ; 
        
        #ifdef PS2
          kbd.begin(4, 3) ;
        #endif

        Serial.begin(115200) ;
        Serial.println(F("Morse Code Keyboard by K6HX, modified by K7MDL for BT keyboard 2025")) ;

        // To test the Pairing code entry, uncomment the following line as pairing info is
        // kept in the nvs. Pairing will then be required on every boot.
        // ESP_ERROR_CHECK(nvs_flash_erase());

        ret = nvs_flash_init();
        if ((ret == ESP_ERR_NVS_NO_FREE_PAGES) || (ret == ESP_ERR_NVS_NEW_VERSION_FOUND)) {
          ESP_ERROR_CHECK(nvs_flash_erase());
          ret = nvs_flash_init();
        }
        ESP_ERROR_CHECK(ret);

        ESP_LOGI(TAG, "BT and BLE device Scan Setup");
        
        btStarted();

        if (bt_keyboard.setup(pairing_handler, keyboard_connected_handler,
                              keyboard_lost_connection_handler)) { // Must be called once
            bt_keyboard.devices_scan(); // Required to discover new keyboards and for pairing
                                      // Default duration is 5 seconds
        }
      
        while (true) 
        {
            #if 0 // 0 = scan codes retrieval, 1 = augmented ASCII retrieval
                uint8_t ch = bt_keyboard.wait_for_ascii_char();
                // uint8_t ch = bt_keyboard.get_ascii_char(); // Without waiting

              if ((ch >= ' ') && (ch < 127)) std::cout << ch << std::flush; 
              else if (ch > 0) {
                std::cout << '[' << +ch << ']' << std::flush;
              }
            #else
                BTKeyboard::KeyInfo inf;
                bt_keyboard.wait_for_low_event(inf);

                // simple decoding fdor Rii K08 BLE mini keyboard, aka Rii model i8+
                //std::cout << "RECEIVED KEYBOARD EVENT: ";
                //for (int n = 0; n < inf.size; n++) {
                if (inf.size == 8) {   // keyboard chars are len = 8, mousr and others are len=4
                    if (inf.keys[2] != 0) {  // filter out key up events where are all 0s.
                    std::cout << std::hex << +inf.keys[2] << ", ";  // do not print key up events
                    }
                }
                
                if (inf.keys[2] == 0x2c) {  // space
                    ch = ' ';
                }

                if (inf.keys[2] == 0x28) {  // enter key
                    ch = ' ';
                }

                if (inf.keys[2] < 0x1e) {
                    ch = inf.keys[2]+93;  // if letter convert to 'a' and shift to upper case
                    if (isalpha(ch)) {
                    if (islower(ch)) 
                        ch = toupper(ch) ;
                    }
                }
                if (inf.keys[2] > 0x1d) {
                    ch_digit = inf.keys[2]+ 0x13;  // if number 1, convert 0x1e to 0x30 for char '1'
                    if (isdigit(ch_digit)) {
                    ch = ch_digit;
                    }
                }

                // filter out key up events
                if (inf.keys[2] != 0)
                    std::cout << ch;

                //std::cout << std::endl;   // sned when we get ener or lf/cr
            #endif

            if (!queueempty())
                send(queuepop()) ;
            
            uint8_t send_CQ = 0;
            if (send_CQ == 1) {
                char c1 = 'C';
                char c2 = 'Q';
                char c3 = ' ';
                send(c1);
                send(c2);
                send(c3);
            } else if (ch != 0) {
                send(ch);
            }

            //delay(1000);

        } // end of while true loop
    } // app_main
} // extern 'C'