//main xmas_v3 program by HybridAir
//compiles to 1946 prog, 46 ram

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

//last button state
bool prevBtn = false;

byte savedStringIndex = 0;                          //array index of the currently selected message string


void setup() {
    switchMessage();
}


void loop() {
    
    bool newBtn = (PINB & (1<<BUTTON));
    if(newBtn != prevBtn) {
        prevBtn = newBtn;
        
        if(newBtn) {
            savedStringIndex++;
            if(savedStringIndex >= TOTAL_STRINGS) {
                savedStringIndex = 0;
            }
            
            switchMessage();
            //need to add software debouncing
        }
    }
    
    
    
    showMessage();
    renderFrame();
    clearFrame();
}