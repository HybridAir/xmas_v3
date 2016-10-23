//handles all advanced visuals, ie text and animations
//TODO: add stuff besides text scrolling

// maps characters to their 4x5 grid 
unsigned long characterMap[59];

char currentMessage[18] = DISPLAY_STRING;
byte currentStringLength = sizeof(currentMessage);


// The offset of our string in the display
int offset = 0;


//byte currentStringLength = 18;

byte currentString = 0;             //currently selected string
bool restartString = true;

// set up a character in the characterMap
void Chr(char theChar, unsigned long value) {
  characterMap[theChar - MAP_START] = value;
  
}



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


// render the string on the given offset
void renderString(char *theString, int offset) {
    
    int currentChar = 0;
    
    //since we dont want to waste resources trying to render every single character at a time
    //start at the last character that is still on the screen, even partially
    //but only do this if the string is currently leaving the screen from the left (not coming in from the right)
    if (offset <= 0) {
        currentChar = abs(offset) / 5;
    }
    
  
    int x = 0;
    char space = 0;
    int extra = 0;
    bool printing = true;
  
  
    while(printing) {
        int bgutt = offset + ((currentChar + x)*4);
        
        
        if(offset + ((currentChar + x)*4) >= -6 + (-1*currentChar)) {
            if(currentChar + x < currentStringLength - 1) {               //make sure there is still a character left to display
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
                renderCharacter(theString[currentChar + x], offset + space + extra + (x*4));
            }
        }
        x++;
            if(x > 2) {
            printing = false;
        }
    }
}
  


// render a character on the given offset
void renderCharacter(char theChar, int charOffset) {

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
}