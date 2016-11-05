//handles text display and other higher level animations
//compiles to 1014 prog, 14 ram

#include <util/delay.h>
#include <avr/pgmspace.h>
#include "mainProgram.h"


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

//char currentString[STRINGLENGTH];
//const char string0[STRINGLENGTH] PROGMEM = "HELLO WORLD";


//switchs the currently displayed string
/* void switchString() {
    firstRun = true;
    offset = 0;
    
    char stringAddress = STRINGLENGTH * currentString;
    byte stringIndex = 0;
    for(char y = stringAddress; y < stringAddress + STRINGLENGTH; y++) {
        byte b = i2c_eeprom_read_byte(0x50, y);
        
        if(b != 0) {
            myString[stringIndex] = b;
            stringIndex++;
            length = stringIndex + 1;
        }
    }
} */



// draw the string on the given offset
void drawString(char *theString, int offset) {
    int currentChar = 0;
    if (offset <= 0) {
        currentChar = abs(offset) / 5;
  //Serial.println(currentChar);
    }
  
  int x = 0;
  char space = 0;
  int extra = 0;
  bool printing = true;
  
  
    while(printing) {
        if(offset + ((currentChar + x)*5) >= -6 + (-1*currentChar)) {
            if(currentChar + x < STRINGLENGTH - 1) {               //make sure there is still a character left to display
                if(x + currentChar > currentChar) {
                    space++;
                }
                else {
                    space = 0;
                }
                
                if(abs(offset) / 5) {
                    extra = 5 * currentChar;
                }
                
                //Serial.println(currentChar + x);
                drawChar(theString[currentChar + x], offset + space + extra + (x*5));
            }
        }
        x++;
            if(x > 2) {
            printing = false;
        }
    }
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
