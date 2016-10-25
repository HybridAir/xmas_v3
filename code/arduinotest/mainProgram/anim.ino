//handles all advanced visuals, ie text and animations
//TODO: add stuff besides text scrolling


//only 4 characters can be possibly displayed at once, so only make room for that many
//the smallest character is 1px wide with a 1px space between each, which gives us 8px (DISPLAY_WIDTH)
//the first byte is the character's width value, the rest are the 5 character map rows
byte charMaps[4][DISPLAY_HEIGHT + 1] = {0};

//the amount of characters that are going to be displayed in the current cycle
byte displayableChars = 0;

//the current starting position in the string that is being displayed, also referred to as the current cycle position
//it defines where the current range of characters from a string that are to be displayed starts at
//the position increments when the last drawn character completely scrolls out of the display
byte stringPosition = 0;




char currentMessage[MAXSTRINGLENGTH] = "";
byte currentStringLength = MAXSTRINGLENGTH;


// The offset of our string in the display
int offset = 0;
int lastOffset = 0;


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
    charAmount = 0;
    
    //calculate the address where the new string is stored at in eeprom
    char stringAddress = MAXSTRINGLENGTH * currentString;
    
    
    
    //for each byte in the string's storage space
    byte stringIndex = 0;
    for(char y = stringAddress; y < stringAddress + MAXSTRINGLENGTH; y++) {
        //read the byte out and store it in our current string container array
        char b = i2c_eeprom_read_byte(0x50, y);
        
        //only read until you get to the end of the string, not the storage space
        if(b != 0) {
            currentMessage[stringIndex] = b;
            stringIndex++;
            currentStringLength = stringIndex + 1;
            
            
        }
    }
    
    
    //show the beginning of the string at offset 0
    //renderString(0);
    //THIS ISN'T NEEDED, SHOW STRING DOES THIS ^^^^^
    //and render stirng does the bottom vvvvvvvv
    
    
/*     //GET THE FIRST FEW CHARACTER MAPS OUT AND READY
    byte i = 0;
    stringIndex = 0;
    
    //while there is still physical room left for characters to be displayed (plus 1 pixel space between)
    //ie if a character fits on the display and its 1 px space fills the last column, dont get anymore
    while(i < DISPLAY_WIDTH - 1) {
        bool skip = false;
        
        //check if we are going to duplicate one of the already downloaded characters
        //for each character that we store on this device
        for(char u = 0; u < 3; u++) {
            //make sure not to compare with the current character
            if(u != stringIndex) {
                //if the new character matches the current downloaded one
                if(charWidths[u][1] == currentMessage[stringIndex]) {
                    //add the width of that one to i, and skip downloading any new ones
                    i = i + charWidths[u][1];
                    skip = true;
                }
            }
        }
        
        //
        if(!skip) {
            readChar(currentMessage[stringIndex], stringIndex);
            //add the character's width plus 1px space to i 
            i = i + charWidths[stringIndex][0] + 1;
        }
    } */
}


//displays and scrolls the current message on the display
void showMessage() {
    
    //if this is the first time running this string, or it is being looped
    if(restartString) {
        
        //display the string at the last saved offset, and hold it there for some time
        renderString(offset);
        if (millis() - currentMillis > FIRSTDELAY) {
            restartString = false;
        }
    }
    //else, just scroll the string
    else {
        
        //display the string at the last saved offset
        renderString(offset);
        
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
                
                currentIndex = 0;
                
                
                offset = DISPLAY_WIDTH;             //not the first time, so bring the text in from the right
                currentMillis = millis();
            }
            
            
        }
        
    }
}




//REMOVE THE *THESTRING, IT'S REDUNDANT
//renders the string on the given offset
//all this really does is tell the character renderer where to put which characters and where
void renderString(int offset) {
    
    if(offset != lastOffset) {
        lastOffset = offset;
        
        //check if we need to move on to the next part of the string 
        //determine this by checking if the first character of the current section index to be displayed is still at least partially visible at the current offset
        //so if the width of the character in the currentIndex, plus the current offset, is greater/equal to 0
        if(charWidths[currentIndex][0] + offset >= 0) {
            //gr8 go display it
            //?????
        }
        //if not, move on to the next character section index fuck
        else{
            currentIndex++;
            
            //MAKE SURE YOU DON'T OVERFLOW THE STRING INDEX HERE YOU FUCK
        }
        
        //Serial.println(currentIndex, DEC);
        //MIGHT BE SOME TROUBLE HERE
        
        
        
        

        //get the first few character maps out and ready below
        
        //start just after the first character, since we already calculated that before
        byte usedColumns = charWidths[currentIndex][0] + offset;
        readChar(currentMessage[currentIndex], 0);
        charIndexes[currentIndex] = currentIndex;
        byte arrayIndex = 1;
        
        Serial.println(charWidths[currentIndex][0], DEC);
        
        //while there is still physical room left for characters to be displayed (plus 1 pixel space between)
        //ie if a character fits on the display and its 1 px space fills the last column, dont get anymore
        while(usedColumns < DISPLAY_WIDTH - 1) {
            bool skip = false;
            
            //check if we are going to duplicate one of the already downloaded characters
            //for each character that we store on this device
            //except for the very first
            //if(arrayIndex != 0){              //NOT NEEDED           
            for(char u = 0; u < 3; u++) {
                //make sure not to compare with the current character
                if(u != arrayIndex) {
                    //if the new character matches the current downloaded one
                    if(charWidths[u][1] == currentMessage[currentIndex + arrayIndex]) {
                        //add the width of that one to i, and skip downloading any new ones
                        usedColumns = usedColumns + charWidths[u][1];
                         charIndexes[arrayIndex] = arrayIndex;
                        skip = true;
                    }
                }
            }
            
            //
            if(!skip) {
                readChar(currentMessage[currentIndex + arrayIndex], arrayIndex);
                charIndexes[arrayIndex] = arrayIndex;
                //add the character's width plus 1px space to i 
                usedColumns = usedColumns + charWidths[currentIndex + arrayIndex][0] + 1;
            }
            
            arrayIndex++;
        }
        
        charAmount = arrayIndex;
    }
    
    
    
    //for each character we are able to display
        //check in each array cell where that character is stored
            //and then display that character
            //and add the width to some extra offset value idk
            
    //actually fuck that
    //when you are getting the charactes out
    //fill a new array with the indexes of each required character, in order
    

    //now we need to get out characters into position for displaying
    
    for(char i = 0; i < charAmount; i++) {
        //char out = charWidths[charIndexes[i][1]];
        renderCharacter(charIndexes[i], offset + 1 + charWidths[charIndexes[i]][0]);
    }
    
    
    
    
    
/*     int x = 0;
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
    }  */
    
    
    
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
void renderCharacter(byte charIndex, int charOffset) {
    
    //find the requested char
    
    
    

/*   unsigned long graphic = characterMap[theChar - MAP_START];

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
    } */
}