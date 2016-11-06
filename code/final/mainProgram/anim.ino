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

bool restartString = true;

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

bool firstRun = true;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

int offset = 0;



void showMessage() {
    //if this is the first time running this string, or it is being looped
    if(firstRun) {
        //display the string at the last saved offset, and hold it there for some time
        drawString(currentString, offset);
        if (millis() - currentMillis > FIRSTDELAY) {
            firstRun = false;
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
            if(abs(offset) >= (18 * 6) - 6){
                //cleanly loop the string
                
                //reset_pins();                       //clear the screen to remove any artifacts
                firstRun = true;                    //restart the scrolling with a dealy
                offset = DISPLAY_WIDTH;             //not the first time, so bring the text in from the right
                //offset = 0;             //not the first time, so bring the text in from the right
                currentMillis = millis();
            }
        }
        
    }
}







// render the string on the given offset
void drawString(char *theString, int offset) {
    int currentChar = 0;
    if (offset <= 0) {
        currentChar = abs(offset) / 5;
  //Serial.println(currentChar);
    }
  

  int x = 0;        //the amount of chars that have been printed in this offset cycle
  char space = 0;
  int extra = 0;
  bool printing = true;
  
  
    while(printing) {
        
        //if we are done drawing the current character????????
        //if the offset and the size of all the currently drawn characers is higher than uh
        //the negative offset of the current character location in the string, plus the space to get it off the display?
        //this is suppsoed to tell the program when there is room to draw a new character
        //I think
        if(offset + ((currentChar + x)*4) >= -6 + (-1*currentChar)) {
            if(currentChar + x < 18 - 1) {               //make sure there is still a character left to display
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
                drawChar(theString[currentChar + x], offset + space + extra + (x*4));
            }
        }
        x++;
        
        //don't bother printing more than 3 chars at once, they wont fit and you'll waste cpu cycles
            if(x > 2) {
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
