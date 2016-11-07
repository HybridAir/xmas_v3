//main xmas_v3 program by HybridAir
//compiles to 1926 prog, 45 ram

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
    //drawChar('2', 0);
    

    
    
    switchMessage();
    
    //drawString(currentString, 0);
    
    //while(true) {
    //renderFrame();
    //}
}


void loop() {
    //draw the frame and do nothing else
        showMessage();
    renderFrame();
    
    for(byte i = 0; i < DISPLAY_HEIGHT; i++) {
        frame[i] = 0;
    }

}