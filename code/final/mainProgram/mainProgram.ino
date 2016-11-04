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


void setup() {
/*     //create a full frame
    for(byte i = 0; i < DISPLAY_HEIGHT; i++) {
        frame[i] = 0b11111111;
    } */
    
    drawChar('2', 0);
    //drawChar(1, -2);
    //drawChar(2, 4);
}


void loop() {
    //draw the frame and do nothing else
    renderFrame();

}