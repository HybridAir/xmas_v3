//basic charlieplex matrix test, goes through all leds
//compiles to 586 bytes
#include <util/delay.h>

#define NUMOFLEDS 40

unsigned long previousMillis = 0;
const long interval = 100; // milliseconds gap between sending a network request.

// I skipped F because F isn't allowed
#define A PA0
#define B PA1
#define C PA2
#define D PA3
#define E PA4
#define G PA5
#define H PA6



// visual array of how the leds are laid out, and the pins they connect to
//cathode first, then anode
int ledGrid[NUMOFLEDS][2] = {

    {A,B},  {B,A},  {C,A},  {D,A},  {E,A},  {G,A},  {H,A},  {A,H},

    {A,C},  {B,C},  {C,B},  {D,B},  {E,B},  {G,B},  {H,B},  {B,H},

    {A,D},  {B,D},  {C,D},  {D,C},  {E,C},  {G,C},  {H,C},  {C,H},

    {A,E},  {B,E},  {C,E},  {D,E},  {E,D},  {G,D},  {H,D},  {D,H},

    {A,G},  {B,G},  {C,G},  {D,G},  {E,G},  {G,E},  {H,E},  {E,H}

};

void setup() {

    //individual led test
    // while(true) {
        // setLed(ledGrid[39]); 
    // }
}



void loop() {

    //display all leds
    for(int i=0;i<NUMOFLEDS;i++){
        setLed(ledGrid[i]);
    }

}


void setLed( int pins[2] ){

  //reset all pins to inputs and LOW
  DDRA = 0;
  PORTA = 0;
  
  //set the output pins as outputs
  DDRA |= (1<<pins[0]);
  DDRA |= (1<<pins[1]);
  
  //since all pins are currently low, just need one operation set set one high
  PORTA |= (1<<pins[1]);
}
