//main xmas_v3 program by HybridAir
//compiles to 2040 prog, 50 ram

//#include <avr/sleep.h>
//#include <avr/power.h>    // Power management
//#include <avr/interrupt.h>
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


bool prevBtn = false;                           //last button state
unsigned long btnTimer = 0;							//time the button has been held down
byte savedStringIndex = 0;                      //array index of the currently selected message string
bool sleepEnabled = true;							//auto sleep enabled by default


void setup() {
    //ACSR |= _BV(ACD);                   			//disable the analog comparator	
	
/* 	if((PINB & (1<<BUTTON))) {						//if the button is held during power up
		sleepEnabled = false;						//disable auto sleep
	} */
     
    switchMessage();
}

unsigned long timer = 0;

void loop() {
    
    bool newBtn = (PINB & (1<<BUTTON));
    if(newBtn != prevBtn) {
        prevBtn = newBtn;
        
        if(newBtn) {                        //if the button was just pressed
            timer = millis();               //start the hold timer
        }
        else {                              //if the button was just released
            //check how long it was held for
            if(millis() >= timer + SLEEP_DELAY) {
                switchMessage();
                //sleep here, above is for testing
            }
            else if(millis() >= timer + ANIM_DELAY) {
                savedStringIndex++;
                if(savedStringIndex >= TOTAL_STRINGS) {
                    savedStringIndex = 0;
                }
                switchMessage();
            }
        }
    }
    
     
    showMessage();
    renderFrame();
    clearFrame();
    
/*     if(sleepEnabled) {								//only check the sleep timer if auto sleeping is enabled
		//sleepTimer();
	} */
}



/* //puts the device to sleep
void sleep() {
	DDRA = 0;      
    PORTA = 0;							//turn all LEDs off to save power

    GIMSK |= _BV(PCIE1);                     //enable Pin Change Interrupts
    PCMSK1 |= _BV(PCINT10);                   //use PB5 as interrupt pin to wake the device back up
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    //set the sleep mode
    sleep_enable();                         //sets the Sleep Enable bit in the MCUCR Register (SE BIT)
    sei();                                  //enable interrupts
    sleep_cpu();                            //sleep

	//wake up here
    cli();                                  //disable interrupts
	//resetMillis();							//reset the millis value since the device is essentially turning off
    PCMSK1 &= ~_BV(PCINT10);                  //turn off PB5 as interrupt pin so it can be used for other things
    sleep_disable();                        //clear SE bit
    sei();                                  //enable interrupts
	
	switchMessage();
} */


//the program goes here directly after waking up
// ISR(PCINT1_vect) {
// }



/* void sleepTimer() {
	if(millis() >= SLEEPTIME) {						//if the current time is greater than the sleeptime value
		sleep();									//go to sleep
	}
} */