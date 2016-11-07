//handles text display and other higher level animations


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


byte currentStringLength = STRINGLENGTH;
byte savedStringIndex = 0;
char currentString[STRINGLENGTH];


const char savedStrings[TOTAL_STRINGS][STRINGLENGTH] PROGMEM = {
    "MERRY CHRISTMAS",
    "HAPPY HOLIDAYS",
    "HAPPY NEW YEAR",
    "FROM, BLAKE",
    "XMAS_V3 2016"
};


bool restartString = true;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

int offset = 0;


//switches to a new message, reads the last selected string out of progmem and resets stuff for the new string
void switchMessage() {
    restartString = true;
    offset = 0;
    
    //read the new string out of progmem until you get to the end of it
    for (byte i = 0; i < STRINGLENGTH; i++) {
        char readChar = pgm_read_byte_near(savedStrings[savedStringIndex] + i);
        
        if(readChar != 0) {
            currentString[i] = readChar;
        }
        else {
            currentStringLength = i + 1;
            break;
        }
    }
}





void showMessage() {
    //if this is the first time running this string, or it is being looped
    if(restartString) {
        //display the string at the last saved offset, and hold it there for some time
        drawString(currentString, offset);
        if (millis() - currentMillis > FIRSTDELAY) {
            restartString = false;
        }
    }
    else {
        drawString(currentString, offset);
        currentMillis = millis();
        if (currentMillis - previousMillis > TEXTSPEED) {
            previousMillis = currentMillis;
            // shift string left one column
            offset--;
            
            //if we are at the end of the string
            if(abs(offset) >= (currentStringLength * 6) - 6){
                //cleanly loop the string
                
                //reset_pins();                       //clear the screen to remove any artifacts
                restartString = true;                    //restart the scrolling with a dealy
                offset = DISPLAY_WIDTH;             //not the first time, so bring the text in from the right
                //offset = 0;             //not the first time, so bring the text in from the right
                currentMillis = millis();
            }
        }
        
    }
}







//draw the string on the given offset
void drawString(char *theString, int offset) {
    
    //this is the current character position in the string, but in pixel form
    //so instead of a string index position number, you get the pixel position
    int currentChar = 0;
    //but only if the offset is negative
    if (offset <= 0) {
        currentChar = abs(offset) / 6;
    }
  

  int x = 0;                //the amount of chars that have been printed in this offset cycle
  char space = 0;           //not really needed here but it works, determines if there is a 1px space between chars
  int extra = 0;            //hell if I know what this does
  bool printing = true;
  
  
    //don't waste time printing/drawing characters if it was established that there are none left?????
    while(printing) {
        
        //if we are done drawing the current character????????
        //if the offset and the size of all the currently drawn characers is higher than uh
        //the negative offset of the current character location in the string, plus the space to get it off the display?
        //this is suppsoed to tell the program when there is room to draw a new character
        //I think
        //if(offset + ((currentChar + x)*4) >= -6 + (-1*currentChar)) {
        if(offset + ((currentChar + x)*5) >= -6 + (-1*currentChar)) {
            
            //ok so now make sure that there is still something left in the string to print
            //string length minus 1 becuase it's 0 indexed
            if(currentChar + x < currentStringLength - 1) {
                
                //if we are not at the beginning of this currest offset display section thing
                //add a 1px space after the character
                if(x + currentChar > currentChar) {
                    space++;
                    //space++;        //I added an extra one
                }
                else {
                    space = 0;
                }
                
                //I don't know what this does but things break if it's gone
                if(abs(offset) / 6) {
                    extra = 6 * currentChar;
                }
                

                drawChar(theString[currentChar + x], offset + space + extra + (x*5));
            }
        }
        x++;
        
        //don't bother printing more than 3 chars at once, they wont fit and you'll waste cpu cycles
            if(x >= 3) {
            printing = false;
        }
    }
}



/* //draws the string at the given offset, but really just tells the character drawer where to draw characters
void drawString(char *theString, int offset) {
    
    //local variable for storing the current character in the string being printed, start at the beginning of the string
    int currentStringIndex = 0;
    
    //since we dont want to waste time trying to draw every single character at a time (which wont even be displayed)
    //start at the oldest character that is still on the screen, even partially
    //but only do this if the string is currently leaving the screen from the left (negative offset only)
    if (offset <= 0) {
        //if the offset is negative, then find the string index that we can start displying the string at
        //this only works because all characters are CHAR_WIDTH pixels wide
        currentStringIndex = abs(offset) / CHAR_WIDTH;
    }
  
  
    
    int displayedChars = 0;      //keeps track of the amount of characters we have displayed so far
    char space = 0;
    int extra = 0;
    bool printing = true;
  
  
    while(printing) {
        
        //current working position in the string, in pixels
        int currentStringPos = ((currentStringIndex + displayedChars) * CHAR_WIDTH);
        
        //the position of the current character in relation to the display, in pixels
        int currentCharPos = (-1*(CHAR_WIDTH + 1)) + (-1*currentStringIndex);
        
        //check if the character we are going to display will even be in display range
        //if the current position of the string with offset is greater than the position of the current character
        if((offset + currentStringPos) >= currentCharPos) {
            
            //if we are not at the end of the string yet
            //TESTME since not all strings are STRINGLENGTH long
            if(currentStringIndex + displayedChars < STRINGLENGTH - 1) {
                
                //add a 1 pixel space after the character if another will be displayed after it
                if(displayedChars + currentStringIndex > currentStringIndex) {
                    space++;
                }
                else {
                    space = 0;
                }
                
                //I forgot what this was for TESTME
                //has to do with string index position working right
                if(abs(offset) / CHAR_WIDTH) {
                    extra = CHAR_WIDTH * currentStringIndex;
                }
                
                drawChar(theString[currentStringIndex + displayedChars], offset + space + extra + (displayedChars*5));
            }
        }
        displayedChars++;
            if(displayedChars > 2) {
            printing = false;
        }
    }
} */



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
