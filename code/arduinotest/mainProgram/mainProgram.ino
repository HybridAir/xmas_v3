//xmas_v3 - HybridAir
//todo: debounce

#include <Wire.h>
#include "mainProgram.h"


//timer stuff
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

//last button state
bool prevBtn = false;


void setup() {
    
    Serial.begin(9600);
    
    pinMode(9, INPUT);
    initDisplay();
    initEeprom();
    switchMessage();     //temp
}


void loop() {
    
    checkBtn();
    showMessage(); 
    
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