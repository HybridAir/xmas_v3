//handles text display and other higher level animations

//character map storage
//characters are in ascii order, and use 25 out of the 32 given bits
const long charMaps[49] PROGMEM = {
    0b0000000000000000000000000,        //+, but we're using it as a space  
    0b0000000000000000010001000,        //,
    0b0000000000011100000000000,        //-
    0b0000000000000000000001000,        //.
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
    0b0111010001111111000110001,        //A
    0b1111110001111101000111111,        //B
    0b0111110000100001000001111,        //C
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
    0b1111010001111101000010000,        //P
    0b0111010001100011001001101,        //Q
    0b1111010001111101001010001,        //R
    0b0111110000011100000111110,        //S
    0b1111100100001000010000100,        //T
    0b1000110001100011000101110,        //U
    0b1000110001010100101000100,        //V
    0b1000110101101011010111011,        //W
    0b1000101010001000101010001,        //X
    0b1000110001011100010000100,        //Y
    0b1111100001011101000011111,        //Z
    0b0000000000000000000011111        //[, but we're using it as a _
};


//message string storage, all strings are a max of 18 chars in length
const char savedStrings[TOTAL_STRINGS][STRINGLENGTH] PROGMEM = {
    "MERRY+CHRISTMAS",
    "HAPPY+HOLIDAYS",
    "HAPPY+NEW YEAR",
    "FROM,+BLAKE",
    "XMAS[V3+2016"
};

byte currentStringLength = STRINGLENGTH;            //length of the currently selected message string
char currentString[STRINGLENGTH];                   //the current string to be displayed

bool restartString = true;
int offset = 0;                                     //curent string location offset value, in pixels

//used for delay timing stuff
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;


//switches to a new message string, reads it out of progmem and resets stuff for in preparation for its display
void switchMessage() {
    
    //new string always start at the beginning
    restartString = true;
    offset = 0;
    
    //read the new string out of progmem until you get to the end of it
    //for each possible character in the string
    for (byte i = 0; i < STRINGLENGTH; i++) { 
        //get the current character out of progmem
        char readChar = pgm_read_byte_near(savedStrings[savedStringIndex] + i);
        
        //stop if you get to the end of the string (strings can still be less than STRINGLENGTH)
        if(readChar != 0) {
            currentString[i] = readChar;
        }
        else {
            currentStringLength = i + 1;
            break;
        }
    }
}


//handles message display timing, looping, etc
//call this often
void showMessage() {
    //if this is the first time running this string, or it is being looped
    
    drawString(offset);
    
    if(restartString) {         //display the string at the last saved offset, and hold it there for some time
        if(millis() - currentMillis > FIRSTDELAY) {
            restartString = false;
        }
    }
    else {                      //display the string normally
        //check if it's time to update the display offset
        currentMillis = millis();
        if (currentMillis - previousMillis > TEXTSPEED) {
            previousMillis = currentMillis;
            // shift string left one column
            offset--;
            
            //if we are at the end of the string, then cleanly loop it
            if(offset <= -1*((currentStringLength * 6) - 6)){
                restartString = true;               //loop with a dealy
                offset = DISPLAY_WIDTH;             //bring the text in from the right
                currentMillis = millis();
            }
        } 
    }
}


//draw the string on the given offset
void drawString(int offset) {
    
    //this is the current character position in the string, but in pixel form
    //so instead of a string index position number, you get the pixel position
    int currentChar = 0;
    
    //but only if the offset is negative
    if (offset <= 0) {
        //add 1 for the space inbetween characters
        currentChar = abs(offset) / (CHAR_WIDTH + 1);
    }
  

    byte x = 0;                 //the amount of chars that have been printed in this offset cycle
    byte space = 0;             //used for adding a 1px space between characters
    int extra = 0;              //hell if I know what this does
    bool printing = true;
  
  
    //don't waste time printing/drawing characters if it was established that there are none left?????
    while(printing) {
        
        //if we are done drawing the current character????????
        //if the offset and the size of all the currently drawn characers is higher than uh
        //the negative offset of the current character location in the string, plus the space to get it off the display?
        //this is suppsoed to tell the program when there is room to draw a new character
        //I think
        if(offset + ((currentChar + x)*CHAR_WIDTH) >= -6 + (-1*currentChar)) {
            
            //ok so now make sure that there is still something left in the string to print
            //string length minus 1 becuase it's 0 indexed
            if(currentChar + x < currentStringLength - 1) {
                
                //if we are not at the beginning of this currest offset display section thing
                //add a 1px space after the character
                if(x + currentChar > currentChar) {
                    space++;
                }
                else {
                    space = 0;
                }
                
                //I don't know what this does but things break if it's gone
                if(abs(offset) / 6) {
                    extra = 6 * currentChar;
                }
                

                drawChar(currentString[currentChar + x], offset + space + extra + (x*CHAR_WIDTH));
            }
        }
        x++;                //go on to the next character to display?
        
        //don't bother printing more than 3 chars at once, they wont fit and you'll waste cpu cycles
            if(x >= 3) {
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
