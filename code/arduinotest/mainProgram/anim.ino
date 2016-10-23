//handles all advanced visuals, ie text and animations


// maps characters to their 4x5 grid 
unsigned long characterMap[59];

char myString[18] = DISPLAY_STRING;
int length = sizeof(myString);


// The offset of our string in the display
int offset = 0;


byte currentStringLength = 18;

char currentString = 0;             //currently selected string
bool firstRun = true;

// set up a character in the characterMap
void Chr(char theChar, unsigned long value) {
  characterMap[theChar - MAP_START] = value;
  
}




void incrementAnim() {
    currentString++;
            if(currentString > NUMOFSTRINGS) {
                currentString = 0;
            }
            
            switchString();
}


void switchString() {
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
}


void showString() {
    //if this is the first time running this string, or it is being looped
    if(firstRun) {
        //display the string at the last saved offset, and hold it there for some time
        renderString(myString, offset);
        if (millis() - currentMillis > FIRSTDELAY) {
            firstRun = false;
        }
    }
    else {
        renderString(myString, offset);
        currentMillis = millis();
        if (currentMillis - previousMillis > TEXTSPEED) {
            previousMillis = currentMillis;
            // shift string left one column
            offset--;
            
            //if we are at the end of the string
            if(abs(offset) >= (length * 5) - 6){
                //cleanly loop the string
                
                reset_pins();                       //clear the screen to remove any artifacts
                firstRun = true;                    //restart the scrolling with a dealy
                offset = DISPLAY_WIDTH;             //not the first time, so bring the text in from the right
                currentMillis = millis();
            }
        }
        
    }
}


// render the string on the given offset
void renderString(char *theString, int offset) {
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
        if(offset + ((currentChar + x)*4) >= -6 + (-1*currentChar)) {
            if(currentChar + x < length - 1) {               //make sure there is still a character left to display
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