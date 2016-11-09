//basic character display test
//compiles to 818 prog, 14 ram
#include <util/delay.h>
#include <avr/pgmspace.h>

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

//character map width
#define CHAR_WIDTH 5

#define PREOFFSET 3

// where does our characterMap start in the ASCII code
#define MAP_START      32


const long charMaps[64] PROGMEM = {
    0b0000000000000000000000000,        //Space
    0b0100001000010000000001000,        //!
    0b0,
    0b0,
    0b0,
    0b0,
    0b0010000100000000000000000,        //'
    0b0,
    0b0,
    0b0,
    0b0,
    0b0000000000000000010001000,        //,
    0b0000000000011100000000000,        //-
    0b0000000000000000000001000,        //.
    0b0000000000000000000000000,
    0b0,
    0b1111110001101011000111111,        //0
    0b0110000100001000010001110,        //1
    0b1111000001011101000011111,        //2
    0b1111100001011110000111111,        //3
    0b1000110001111110000100001,        //4
    0b1111110000011100000111110,        //5
    0b1111010000111111000111111,        //6
    0b1111100001000100010001000,        //7
    0b1111110001011101000111111,        //8
    0b1111110001111110000101111,        //9
    0b0000000100000000010000000,        //:
    0b0,
    0b0,
    0b0,
    0b0,
    0b0,
    0b1111000001111011010101110,        //@
    0b1111010001111111000110001,        //A
    0b1111110001111101000111111,        //B
    0b0111110000100001000011111,        //C
    0b1111010001100011000111110,        //D
    0b1111110000111101000011111,        //E
    0b1111110000111101000010000,        //F
    0b0111110000101111000101111,        //G
    0b1000110001111111000110001,        //H
    0b0111000100001000010001110,        //I
    0b0111100001000011000111111,        //J
    0b1000110010111001001010001,        //K
    0b1000010000100001000011111,        //L
    0b1101110101101011010110001,        //M
    0b1000111001101011001110001,        //N
    0b0111010001100011000101110,        //O
    0b1111010001111111000010000,        //P
    0b0111010001100011001001101,        //Q
    0b1111110001111111001010001,        //R
    0b0111110000011100000111110,        //S
    0b1111100100001000010000100,        //T
    0b1000110001100011000111110,        //U
    0b1000110001010100101000100,        //V
    0b1000110101101011010111011,        //W
    0b1000101010001000101010001,        //X
    0b1000110001011100010000100,        //Y
    0b1111100001011101000011111,        //Z
    0b0,        
    0b0,        
    0b0,        
    0b0,        
    0b0000000000000000000011111         //_
};


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
/*     //create a full frame
    for(byte i = 0; i < DISPLAY_HEIGHT; i++) {
        frame[i] = 0b11111111;
    } */
    
    drawChar('2', 0);
    //drawChar(1, -2);
    //drawChar(2, 4);
}


void loop() {
    //draw the frame and do nothing else
    renderFrame();

}



//takes the specified character and draws it on the frame array
//void drawChar(char theChar, int charOffset) {
void drawChar(char theChar, int offset) {
    
    //by default, the characters are aligned to the right of the display, so this will fix that
    int finalOffset = offset - PREOFFSET;

    //get the specified character map out of progmem
    long currentCharMap = pgm_read_dword(&(charMaps[theChar - MAP_START]));

    //go through each character map row
    for (byte row = DISPLAY_HEIGHT; row >= 1; row--) {
        
        //take the character map and AND out the lowest 5 bits
        long charRow = currentCharMap & 0b11111;
        
        //take the ANDed result and put it in the correct frame row
        if(finalOffset > 0) {
            frame[row - 1] |= (byte)charRow >> finalOffset;
        }
        else {
            frame[row - 1] |= (byte)charRow << abs(finalOffset);
        }
        
        
        //then shift the local map over 5 bits
        currentCharMap = currentCharMap >> CHAR_WIDTH;
    }
}


void renderFrame() {
        //for each display section
    for(byte i = 0; i < NUM_SECTIONS; i++) {
        //the above only works because each charlie pin used is in numerical order
        //this probably wont work if you start changing pinouts but it saved me like 30 bytes so
        
        //reset all pins to inputs and LOW
        //CHANGE THIS LATER since other functions will need to use portA too
        DDRA = 0;      
        PORTA = 0;
        
        //set this section's cathode pin as an output, its state is still low from before
        DDRA = (1<<i);
        
        //go through each led in the display
        for(byte led = 0; led < NUM_LEDS; led++) {
            //get the cathode value out of the current ledgrid entry
            byte currentCathode = pgm_read_byte(&(ledGrid[led][0]));
            
            //if the cathode cell value in the current ledgrid entry is the displaySection that we are looking for
            if(currentCathode == i) {
                
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