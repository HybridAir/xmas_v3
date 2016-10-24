//handles all advanced visuals, ie text and animations
//TODO: add stuff besides text scrolling


//try combining these two if free memory or storage gets low
// byte charMap0[5] = {0,0,0,0,0};
// byte charMap1[5] = {0,0,0,0,0};
// byte charMap2[5] = {0,0,0,0,0};


//only 3 unique characters can be possibly displayed at once, since there a 1 px space between characters
//so we only need room to temporarily store at most 3 characters
byte charMaps[3][DISPLAY_HEIGHT] = {
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0}
};

byte charWidths[3] = {
    0, 0, 0
};

// byte charWidth0 = 0;
// byte charWidth1 = 0;
// byte charWidth2 = 0;


char currentMessage[18] = "";
byte currentStringLength = sizeof(currentMessage);


// The offset of our string in the display
int offset = 0;


//byte currentStringLength = 18;

byte currentString = 0;             //currently selected string/animation
bool restartString = true;
byte currentIndex = 0;          //current location in the string



//switches the currently playing animation
void incrementAnim() {
    currentString++;
    if(currentString > NUMOFSTRINGS) {
        currentString = 0;
    }
    
    switchMessage();
}


//reads a new message string out of eeprom, and prepares it for display
void switchMessage() {
    
    //reset starting variables
    restartString = true;
    offset = 0;
    currentIndex = 0;
    
    //calculate the address where the new string is stored at in eeprom
    char stringAddress = STRINGLENGTH * currentString;
    
    //for each byte in the string's storage space
    byte stringIndex = 0;
    for(char y = stringAddress; y < stringAddress + STRINGLENGTH; y++) {
        //read the byte out and store it in our current string container array
        byte b = i2c_eeprom_read_byte(0x50, y);
        
        //only read until you get to the end of the string, not the storage space
        if(b != 0) {
            currentMessage[stringIndex] = b;
            stringIndex++;
            currentStringLength = stringIndex + 1;
        }
    }
    
    
    
    
    //GET THE FIRST FEW CHARACTER MAPS OUT AND READY
    byte i = 0;
    stringIndex = 0;
    
    //while there is still physical room left for characters to be displayed (plus 1 pixel space between)
    //ie if a character fits on the display and its 1 px space fills the last column, dont get anymore
    while(i < DISPLAY_WIDTH - 1) {
        //read the char map out to our global array
        readChar(currentMessage[stringIndex], stringIndex);
        //add the character's width plus 1px space to i 
        i = i + charWidths[stringIndex] + 1;
    }
    
    
    
    
/*     byte i = 0;
    stringIndex = 0;
    //while there is still physical room left for characters to be displayed (with 1 pixel space)
    //so if a character fits on the display and it's 1 px space fills the last column, dont get anymore
    while(i < DISPLAY_WIDTH - 1) {
        
        byte currentChar = currentMessage[stringIndex];
        
        //get the current character data we need off of the eeprom
        byte charWidth;
        unsigned long rawChar;
        readChar(currentChar, &rawChar, &charWidth);
        charWidths[stringIndex] = charWidth;
        
        //depending on the value of charWidths[stringIndex], make a binary value the same length to use later
        //ie if charWidths[stringIndex] is 4, then output 0b1111
        byte widthMerge = 0x1;
        for(int x = 0; x < charWidths[stringIndex]; x++) {
            widthMerge = (widthMerge << 0x1) | 0x1;
        }

        
        //for each ascending row of the display (since all characters are DISPLAY_HEIGHT tall)
        for(byte u = DISPLAY_HEIGHT; u > 0; u--) {
            //take out the lowest charWidths[stringIndex] amount of bits, and put them in the global map location
            charMaps[stringIndex][u] = rawChar & widthMerge;
            //move the local over charWidths[stringIndex] amount of bits
            rawChar = rawChar >> widthMerge;
            //this is done in decending order because the lowest bit in the charMap is at the bottom of the map
        }
        
        stringIndex++;
        i = i + charWidths[stringIndex] + 1;
        
    } */
}


//displays and scrolls the current message on the display
void showMessage() {
    
    //if this is the first time running this string, or it is being looped
    if(restartString) {
        //display the string at the last saved offset, and hold it there for some time
        renderString(currentMessage, offset);
        if (millis() - currentMillis > FIRSTDELAY) {
            restartString = false;
        }
    }
    //else, just scroll the string
    else {
        
        //display the string at the last saved offset
        renderString(currentMessage, offset);
        
        //after a preset delay, decrement the offset to scroll the string
        currentMillis = millis();
        if (currentMillis - previousMillis > TEXTSPEED) {
            previousMillis = currentMillis;
            // shift string left one column
            offset--;
            
            //if we are at the end of the string
            if(abs(offset) >= (currentStringLength * 5) - 6){
                //cleanly loop the string
                
                reset_pins();                       //clear the screen to remove any artifacts
                restartString = true;                    //restart the scrolling with a dealy
                offset = DISPLAY_WIDTH;             //not the first time, so bring the text in from the right
                currentMillis = millis();
            }
        }
        
    }
}


//renders the string on the given offset
//all this really does is tell the character renderer where to put which characters and where
void renderString(char *theString, int offset) {
    
    
    //currentIndex?
    
    
    //check if the currentIndex character is still on the display
    
    //if the width of the character in the currentIndex, plus the current offset is greater/equal to 0
    //which means if the first character to be displayed is still able to fit on the display
/*     if() {
        
    } */
    
    //then diusplay
    //else incremebet the index
    
    
    
    
    //get char widths and maps out  
    //for each pixel in the display's width
/*     for(char i = 0; i < DISPLAY_WIDTH; i++) {
        
    } */
    
    
     int x = 0;
    char space = 0;
    int extra = 0;
    bool printing = true;
  
  
    while(printing) {
        
        
        if(offset + ((currentIndex + x)*4) >= -6 + (-1*currentIndex)) {
            if(currentIndex + x < currentStringLength - 1) {               //make sure there is still a character left to display
                if(x + currentIndex > currentIndex) {
                    space++;
                }
                else {
                    space = 0;
                }
                
                if(abs(offset) / 5) {
                    extra = 5 * currentIndex;
                }
                
                //Serial.println(currentIndex + x);
                //renderCharacter(theString[currentIndex + x], offset + space + extra + (x*4));
            }
        }
        x++;
            if(x > 2) {
            printing = false;
        }
    } 
    
    
    
/*     //since we dont want to waste resources trying to render every single character all the time
    //start at the last character that is still at least partially on the display
    if (offset <= 0) {
        //only do this if the screen is scrolling out on the left, not entering in on the right (ie after a reset)
        //set the current string location to thefuck
        //cant do this because the characters are not a single fixed width
        //figure out something with the offset
        currentIndex = abs(offset) / 5;
    } */
    
    
/*     int currentIndex = 0;
    
    //since we dont want to waste resources trying to render every single character at a time
    //start at the last character that is still on the screen, even partially
    //but only do this if the string is currently leaving the screen from the left (not coming in from the right)
    if (offset <= 0) {
        currentIndex = abs(offset) / 5;
    }
    
  
    int x = 0;
    char space = 0;
    int extra = 0;
    bool printing = true;
  
  
    while(printing) {
        //int bgutt = offset + ((currentIndex + x)*4);
        
        
        if(offset + ((currentIndex + x)*4) >= -6 + (-1*currentIndex)) {
            if(currentIndex + x < currentStringLength - 1) {               //make sure there is still a character left to display
                if(x + currentIndex > currentIndex) {
                    space++;
                }
                else {
                    space = 0;
                }
                
                if(abs(offset) / 5) {
                    extra = 5 * currentIndex;
                }
                
                //Serial.println(currentIndex + x);
                renderCharacter(theString[currentIndex + x], offset + space + extra + (x*4));
            }
        }
        x++;
            if(x > 2) {
            printing = false;
        }
    } */
}
  


// render a character on the given offset
/* void renderCharacter(char theChar, int charOffset) {

  unsigned long graphic = characterMap[theChar - MAP_START];

    //go through each bit in the current character map and see if it needs to be displayed
    //iterate through each row, then column
    for (byte y = 5; y >= 1; y--) {
        for (byte x = 4; x >= 1; x--) {
            // 3 - x to reverse order
            int offset = x + charOffset;
            if(offset > 0 && offset < 9) {
                setPixel(offset, y, graphic & 0x1);
            }
            graphic = graphic >> 1;
        }
    }
} */