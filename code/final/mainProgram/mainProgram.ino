//main xmas_v3 program by HybridAir
//compiles to 1014 prog, 14 ram

#include <util/delay.h>
#include <avr/pgmspace.h>
#include "mainProgram.h"

//the current frame to be displayed
byte frame[DISPLAY_HEIGHT] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};


char currentString[STRINGLENGTH];
const char string0[STRINGLENGTH] PROGMEM = "HELLO WORLD";



void setup() {
    //drawChar('2', 0);
    
    for (byte i = 0; i < STRINGLENGTH; i++) {
        currentString[i] =  pgm_read_byte_near(string0 + i);
    }
    
    drawString(currentString, -1);
}


void loop() {
    //draw the frame and do nothing else
    renderFrame();

}