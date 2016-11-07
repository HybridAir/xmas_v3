//main xmas_v3 program by HybridAir
//compiles to 1934 prog, 46 ram

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
bool btnTimerFlag = false;							//lets the program know if the user is holding the button down
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


void loop() {
    
    checkBtn();
    
/*     bool newBtn = (PINB & (1<<BUTTON));
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
    } */
    
    
    
    showMessage();
    renderFrame();
    clearFrame();
    
/*     if(sleepEnabled) {								//only check the sleep timer if auto sleeping is enabled
		//sleepTimer();
	} */
}



//gets the current button state, does not work when sleeping
void checkBtn() {
	byte newBtn = (PINB & (1<<BUTTON));								//get the current/new button state (1 or 0)
	
	if(newBtn != prevBtn) {											//if this new state is different from the last one
		prevBtn = newBtn;											//record the new button state as the new previous one
	
		if(newBtn) {												//if the button just went from LOW to HIGH
            
			btnTimer = millis();									//get the new start time for the button timer
			btnTimerFlag = true;									//start the button timer
            
		}
		else {														//if the button just went from HIGH to LOW
            
			//the button has not been held long enough to go to sleep
			//but it may also not have been held long enough to pass the debouncing test and switch the animation
			
			if(btnTimerFlag) {										//make sure the button timer is even running before we check that
				//the above is needed for a dumb edge case when the button is released when waking up
                
					//unsigned long currentMillis2 = millis();
/* 				if(currentMillis2 >= (btnTimer + 2000)) {			//if the button has been held for at least SLEEP_DELAY ms
					btnTimerFlag = false;							//stop the button timer
					//sleep();										//go to sleep
                    switchMessage();
				} */
				if(millis() - btnTimer >= 100) {		//if the button has been HIGH for at least ANIM_DELAY ms
				//else if(currentMillis2 - btnTimer > 25) {		//if the button has been HIGH for at least ANIM_DELAY ms
					btnTimerFlag = false;							//stop the button timer
					
                    savedStringIndex++;
                    if(savedStringIndex >= TOTAL_STRINGS) {
                        savedStringIndex = 0;
                    }
                    
                    switchMessage();
                    
				}
				else {												//the button press failed the debounce test (not held long enough)
					btnTimerFlag = false;							//stop the button timer
				}
			}
		}
	}
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