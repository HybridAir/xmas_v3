//xmas_v3 - HybridAir

#include <Wire.h>
#include "mainProgram.h"


//timer stuff
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

//last button state
bool prevBtn = false;


void setup() {
    
    pinMode(9, INPUT);
    initDisplay();
    initEeprom();
    switchString();     //temp
}


void loop() {
    
    checkBtn();
    showString(); 
    
}


//checks the button for updates, and processes them
void checkBtn() {
    bool newBtn = digitalRead(9);
    if(newBtn != prevBtn) {
        prevBtn = newBtn;
        if(newBtn) {
            incrementAnim();
            //need to add software debouncing here
        }
    }
}