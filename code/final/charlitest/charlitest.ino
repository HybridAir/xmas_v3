//basic charlieplex matrix test, goes through all leds
//compiles to 650 prog, 14 ram
#include <util/delay.h>

//total number of leds in the display
#define NUM_LEDS 40

//display section names with their cooresponding charpliex pins
//F isn't used becuase AVR uses it for frequency stuff
#define A PA0
#define B PA1
#define C PA2
#define D PA3
#define E PA4
#define G PA5
#define H PA6

//display size
#define DISPLAY_WIDTH  8
#define DISPLAY_HEIGHT 5

//the amount of charlieplexed display sections
#define NUM_SECTIONS 7


//visual array of how the leds are laid out, and the pins that they each connect to
//cathode first, then anode
const byte ledGrid[NUM_LEDS][2] PROGMEM = {

    {A,B},  {B,A},  {C,A},  {D,A},  {E,A},  {G,A},  {H,A},  {A,H},

    {A,C},  {B,C},  {C,B},  {D,B},  {E,B},  {G,B},  {H,B},  {B,H},

    {A,D},  {B,D},  {C,D},  {D,C},  {E,C},  {G,C},  {H,C},  {C,H},

    {A,E},  {B,E},  {C,E},  {D,E},  {E,D},  {G,D},  {H,D},  {D,H},

    {A,G},  {B,G},  {C,G},  {D,G},  {E,G},  {G,E},  {H,E},  {E,H}
    
};


//the current frame to be displayed
byte frame[DISPLAY_HEIGHT] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};


void setup() {
    //create a full frame
    for(byte i = 0; i < DISPLAY_HEIGHT; i++) {
        frame[i] = 0b11111111;
    }
}


void loop() {
    //draw the frame and do nothing else

    //for each display section
    for(byte i = 0; i < NUM_SECTIONS; i++) {
        //the above only works because each charlie pin used is in numerical order
        //this probably wont work if you start changing pinouts but it saved me like 30 bytes so
        
        //reset all pins to inputs and LOW
        //CHANGE THIS LATER since other functions will need to use portA too
        DDRA = 0;      
        PORTA = 0;
        
        //go through each led in the display
        for(byte led = 0; led < NUM_LEDS; led++) {
            //get the cathode value out of the current ledgrid entry
            byte currentCathode = pgm_read_byte(&(ledGrid[led][0]));
            
            //if the cathode cell value in the current ledgrid entry is the displaySection that we are looking for
            if(currentCathode == i) {
                
                //set this section's cathode pin as an output, its state is still low from before
                DDRA |= (1<<currentCathode);
                
                //check if the current led is supposed to be lit in the current frame
                if(getPixel(led + 1)) {         //getPixel is not 0 indexed, so add 1
                
                    //get the current anode value out of the current ledgrid entry
                    byte currentAnode = pgm_read_byte(&(ledGrid[led][1]));
                    
                    //set the anode pin as an output and HIGH
                    DDRA |= (1<<currentAnode);
                    PORTA |= (1<<currentAnode);
                }  
            }
        }
        
        _delay_ms(1);           //disgusting hack needed for brightness equalization????????
        
    }
}


//give this an led pixel number (1-40), and it will return if it's lit or not, NOT 0 indexed
bool getPixel(byte pixelIn) {
    //get the row that the pixel is on
    byte row = pixelIn / DISPLAY_WIDTH;
    
    //make sure to not to wrap the last pixel in the row
    if(pixelIn % DISPLAY_WIDTH == 0) {
        row = row - 1;
    }
    
    //get the column that the pixel is on
    byte col = pixelIn - (row * DISPLAY_WIDTH);
    
    //get the row from the frame that we need out, and return the status of the specified pixel within
    byte frameRow = frame[row];
    return (frameRow >> (DISPLAY_WIDTH - col)) & 0x1;
}