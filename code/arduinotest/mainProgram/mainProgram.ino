//basic charlieplex matrix test, scrolls a string of text forever


#include <Wire.h> //I2C library
#include "mainProgram.h"


// maps characters to their 4x5 grid 
unsigned long characterMap[59];

char myString[18] = DISPLAY_STRING;
int length = sizeof(myString);

//timer stuff
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

// The offset of our string in the display
int offset = 0;


bool firstRun = true;

bool prevBtn = false;
char currentString = 0;             //currently selected string

byte currentStringLength = 18;

// set up a character in the characterMap
void Chr(char theChar, unsigned long value) {
  characterMap[theChar - MAP_START] = value;
  
}

void setup() {
   initLeds();
   
   Chr('A', 0b01101001111110011001);
  Chr('B', 0b11101001111010011110);
  Chr('C', 0b01111000100010000111);
  Chr('D', 0b11101001100110011110);
  Chr('E', 0b11111000111010001111);
  Chr('F', 0b11111000111010001000);
  Chr('G', 0b01111000101110010110);
  Chr('H', 0b10011001111110011001);
  Chr('I', 0b01110010001000100111);
  Chr('J', 0b01110010001010100100);
  Chr('K', 0b10011010110010101001);
  Chr('L', 0b10001000100010001111);
  Chr('M', 0b10011111111110011001);
  Chr('N', 0b10011101101110011001);
  Chr('O', 0b01101001100110010110);
  Chr('P', 0b11101001111010001000);
  Chr('Q', 0b01101001101101100001);
  Chr('R', 0b11101001111010101001);
  Chr('S', 0b11111000111100011111);
  Chr('T', 0b01110010001000100010);
  Chr('U', 0b10011001100110010110);
  Chr('V', 0b10011001100110100100);
  Chr('W', 0b10011001111111110110);
  Chr('X', 0b10011001011010011001);
  Chr('Y', 0b10011001011000101100);
  Chr('Z', 0b11110001001001001111);
  Chr(' ', 0b00000000000000000000);
  Chr('!', 0b01000100010000000100);
  Chr('_', 0b00000000000000001111);
  Chr('0', 0b11111001100110011111);
  Chr('1', 0b01100010001000100111);
  Chr('2', 0b11110001111010001111);
  Chr('3', 0b11100001011000011110);
  Chr('6', 0b11101000111110011111);

  
  pinMode(9, INPUT);
  
  initEeprom();
  switchString();
}





void loop() {
    
    bool newBtn = digitalRead(9);
    if(newBtn != prevBtn) {
        prevBtn = newBtn;
        
        if(newBtn) {
            currentString++;
            if(currentString > NUMOFSTRINGS) {
                currentString = 0;
            }
            
            switchString();
            //need to add software debouncing here
        }
    }
    
    showString();
    
    
    
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