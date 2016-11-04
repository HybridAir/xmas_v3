//basic charlieplex matrix test, goes through all leds
//compiles to 684 prog, 22 ram
#include <util/delay.h>

//total number of leds in the display
#define NUM_LEDS 40

//charpliex pins and section names
//F isn't used becuase AVR uses it for freqency stuff
#define A PA0
#define B PA1
#define C PA2
#define D PA3
#define E PA4
#define G PA5
#define H PA6

#define DISPLAY_WIDTH  8
#define DISPLAY_HEIGHT 5

//the amount of charlieplexed display sections
#define NUM_SECTIONS 7

const byte charlieSections[NUM_SECTIONS] = {A, B, C, D, E, G, H};



// visual array of how the leds are laid out, and the pins they connect to
//cathode first, then anode
const byte ledGrid[NUM_LEDS][2] PROGMEM = {

    {A,B},  {B,A},  {C,A},  {D,A},  {E,A},  {G,A},  {H,A},  {A,H},

    {A,C},  {B,C},  {C,B},  {D,B},  {E,B},  {G,B},  {H,B},  {B,H},

    {A,D},  {B,D},  {C,D},  {D,C},  {E,C},  {G,C},  {H,C},  {C,H},

    {A,E},  {B,E},  {C,E},  {D,E},  {E,D},  {G,D},  {H,D},  {D,H},

    {A,G},  {B,G},  {C,G},  {D,G},  {E,G},  {G,E},  {H,E},  {E,H}
    
};


byte frame[DISPLAY_HEIGHT] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};


//give this an led pixel number (1-40), and it will return if it's lit or not, NOT 0 indexed
bool getPixel(byte pixelIn) {
    byte row = pixelIn / DISPLAY_WIDTH;
    if(pixelIn % DISPLAY_WIDTH == 0) {
        row = row - 1;
    }
    
    byte col = pixelIn - (row * DISPLAY_WIDTH);
    
    byte frameRow = frame[row];
    return (frameRow >> (DISPLAY_WIDTH - col)) & 0x1;
}


void setup() {


    
    
    //create a full frame
    for(byte i = 0; i < DISPLAY_HEIGHT; i++) {
        frame[i] = 0b11111111;
    }
    
    
/*         //individual led test
    while(true) {
        if(getPixel(8)) {
            setLed(7); 
        }
        
    } */
}


void loop() {

    //for each charlie section
    for(byte i = 0; i < NUM_SECTIONS; i++) {
        
        //get the charliesection value that we want out (A-H)
        byte currentSection = charlieSections[i];
        
        //reset all pins to inputs and LOW
        //INPUTS ARE IMPORTANT
        DDRA = 0;      
        PORTA = 0;
        
        
        //go through all leds in the display
        for(byte led = 0; led < NUM_LEDS; led++) {
            //get the cathode value out of the current ledgrid entry
            byte currentCathode = pgm_read_byte(&(ledGrid[led][0]));
            
            //if the first cell in the current led entry is the section we are looking for
            if(currentCathode == currentSection) {
                
                //set the cathode in as an output, state is still low from before
                DDRA |= (1<<currentCathode);
                
                
                //check if this entry is supposed to be lit
                if(getPixel(led + 1)) {         //getPixel is not 0 indexed, add 1
                    //get the anode value that we need out
                    byte currentAnode = pgm_read_byte(&(ledGrid[led][1]));
                    
                    //set the anode pin as an output and HIGH
                    DDRA |= (1<<currentAnode);
                    PORTA |= (1<<currentAnode);         //set it m8
                }
                
                
            }
        }
        
        _delay_ms(1);           //disgusting hack needed for brightness equalization????????
        
    }

}


void setLed( byte ledIn ){
    
  byte pins[2];
  pins[0] = pgm_read_byte(&(ledGrid[ledIn][0]));		//get the LED the user wants to be lit out of PROGMEM
  pins[1] = pgm_read_byte(&(ledGrid[ledIn][1]));		//get the LED the user wants to be lit out of PROGMEM

  //reset all pins to inputs and LOW
  DDRA = 0;
  PORTA = 0;
  
  //set the output pins as outputs
  DDRA |= (1<<pins[0]);
  DDRA |= (1<<pins[1]);
  
  //since all pins are currently low, just need one operation set set one high
  PORTA |= (1<<pins[1]);
}

