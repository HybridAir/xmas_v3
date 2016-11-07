//main xmas_v3 program by HybridAir
//compiles to 1900 prog, 45 ram

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
    switchMessage();
}


void loop() {
    showMessage();
    renderFrame();
    clearFrame();
}