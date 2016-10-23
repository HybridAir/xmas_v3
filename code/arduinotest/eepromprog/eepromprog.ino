//writes strings and stuff to the eeprom, mainly used for testing


#include <Wire.h> //I2C library

#define HI_STRINGADDR 0x00
#define LO_STRINGADDR 0x00
#define STRINGLENGTH 18

#define HI_CHARADDR 0x01        //starting high address for character maps
#define LOW_CHARADDR 0x00       //starting low address in eeprom for character maps
#define MAPSIZE 4              //the amount of bytes each character map entry uses in the eeprom


// where does our characterMap start in the ASCII code
#define MAP_START      32

//strings are stored from 0x00 to 0x7F
//enough room for up to 7, 18 byte strings, using 126 bytes of space with 127 total

//character maps are stored from 0x80
//each character map needs up to 25 bits of space, plus 3 for the width value, so 28 bits, then rounded up to 32
//so each character map has 4 bytes allocated to it

//sorted by ascii table decimal values 32 to 126, - 32 so it starts at 0 in eeprom
//enough room for 94 character maps, using 376 bytes of space
//yeah this wont all fit in the attiny44's 256 byte eeprom, we need this external chip


void setup() {
    Serial.begin(9600);
    Wire.begin();
    
    
    unsigned long character = 0b01101001111110011001;
    
    writeChar('A', character, 4);
    delay(10);
    readChar('A');
    
    
    //strings();
    Serial.println("Done");

}


void loop() {}


//reads a character out of the eeprom, and displays it in serial
void readChar(char theChar) {
    //FIND WHERE THE CHARACTER LIVES
    //subtract 32 from the given character to get the character's 0-indexed position
    byte characterPos = theChar - MAP_START;
    
    //the character's address in eeprom starts at 0x0100, plus the character's position in ascii, times MAPSIZE (4)
    //so every 4 bytes, there is a character map
    byte hiAddr = HI_CHARADDR;
    byte lowAddr = LOW_CHARADDR;
    
    //if the character we are writing will not fit in the first 256 bytes (256/4=64) in the lower character map address
    //(this means only 64 character maps will fit in a single upper address)
    if(characterPos >= 64) {
        //increase the upper address by 1
        hiAddr++;
        
        //set the lower address according to position in the 0-indexed ascii table
        //then subtract 64 since we had to increase our high address
        lowAddr = LOW_CHARADDR + (characterPos * MAPSIZE);
        lowAddr = lowAddr - 64;
    }
    else {
        //else, just set the lower address according to position in the 0-indexed ascii table
        lowAddr = LOW_CHARADDR + (characterPos * MAPSIZE);
    }
    
    //combine both high and low addresses together to get your final address
    int characterAddr = hiAddr << 8;
    characterAddr = characterAddr | lowAddr;
    
    //prepare the array for storing the data we get from the eeprom
    unsigned long input[MAPSIZE] = {0,0,0,0};
    
    //for each byte in a character map
    for(char i = 0; i < MAPSIZE; i++) {
        //read the byte from the eeprom at the specified address and save it
        byte b = i2c_eeprom_read_byte(0x50, characterAddr + i);
        input[i] = b;
        delay(10);
    }
    
    //compine the array into a single long
    unsigned long output = input[0] << 24;
    output = output | (input[1] << 16);
    output = output | (input[2] << 8);
    output = output | input[3];
    
    //get the width value out
    unsigned long width = output >> (MAPSIZE * 8) - 3;
    byte charLength = width * 5;
    
    
    //get the character map value out
    unsigned long charMap = output & 0x1FFFFFFF;
    
    Serial.print("Read: ");
    Serial.println(output, HEX);
    
    
   
/*     byte cols = 0;
    
    //for each bit in the character map
    for(int i = MAPSIZE - charLength; i < MAPSIZE; i++) {
        if(cols < width - 1) {
            Serial.print(charMap & 0x80000000, BIN);
            cols++;
        }
        else {
            Serial.println(charMap & 0x80000000, BIN);
            cols = 0;
        }   

        charMap = charMap * 10;
    } */
    
}


//writes a character map to the eeprom at the specific character position
void writeChar(char theChar, unsigned long charMap, byte width) {
    
    //subtract 32 from the given character to get the character's 0-indexed position
    byte characterPos = theChar - MAP_START;
    
    //the character's address in eeprom starts at 0x0100, plus the character's position in ascii, times MAPSIZE (4)
    //so every 4 bytes, there is a character map
    byte hiAddr = HI_CHARADDR;
    byte lowAddr = LOW_CHARADDR;
    
    //if the character we are writing will not fit in the first 256 bytes (256/4=64) in the lower character map address
    //(this means only 64 character maps will fit in a single upper address)
    if(characterPos >= 64) {
        //increase the upper address by 1
        hiAddr++;
        
        //set the lower address according to position in the 0-indexed ascii table
        //then subtract 64 since we had to increase our high address
        lowAddr = LOW_CHARADDR + (characterPos * MAPSIZE);
        lowAddr = lowAddr - 64;
    }
    else {
        //else, just set the lower address according to position in the 0-indexed ascii table
        lowAddr = LOW_CHARADDR + (characterPos * MAPSIZE);
    }
    
    //combine both high and low addresses together to get your final address
    int characterAddr = hiAddr << 8;
    characterAddr = characterAddr | lowAddr;

    //start the output by putting the lowest 3 bits of the width at the beginning
    //shift the width value left 32 bits, minus 3
    //only need 3 bits since the highest width value we will be using is probably 5
    unsigned long output = width;
    output = output << ((MAPSIZE * 8) - 3);
    
    //merge the width and charmap values together
    //the charmap will be all the way to the right because of how variables work
    //the width value is going to tell the program how far to look right for the beginning
    //total size still is 32 bits, or 4 bytes
    output = output | charMap;
    
    //split the output into bytes, and put them into an array
    byte outArray[4] = {
        (output & 0xff000000UL) >> 24,
        (output & 0x00ff0000UL) >> 16,
        (output & 0x0000ff00UL) >>  8,
        (output & 0x000000ffUL)
    };
    
    Serial.println(output, HEX);
    
    //for each byte in the character map
    for(char i = 0; i < MAPSIZE; i++) {
        //write each byte to the calculated position in the eeprom
        i2c_eeprom_write_byte(0x50, characterAddr + i, outArray[i]);
        delay(10);
    }
}


void strings() {
        char string0[STRINGLENGTH] = "MERRY CHRISTMAS";
    char string1[STRINGLENGTH] = "HAPPY HOLIDAYS";
    char string2[STRINGLENGTH] = "HAPPY NEW YEAR";
    char string3[STRINGLENGTH] = "BY BLAKE STRATTON";
    char string4[STRINGLENGTH] = "XMAS_V3 2016";
    
    
    writeString(string0, STRINGLENGTH * 0);
    writeString(string1, STRINGLENGTH * 1);
    writeString(string2, STRINGLENGTH * 2);
    writeString(string3, STRINGLENGTH * 3);
    writeString(string4, STRINGLENGTH * 4); 

    delay(10);
    
/*     for(char i = 0; i <= 4; i++) {
        char LO_STRINGADDR = 18 * i;
        //Serial.println(18 * i, DEC);
        for(char y = LO_STRINGADDR; y < LO_STRINGADDR + 18; y++) {
            byte b = i2c_eeprom_read_byte(0x50, y);
            Serial.print((char)b); //print content to serial port
            //Serial.print(".");
            //Serial.print(y, DEC);
        }
        
    } */
}





  void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
    int rdata = data;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(rdata);
    Wire.endTransmission();
  }
  
    byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
    byte rdata = 0xFF;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) rdata = Wire.read();
    return rdata;
  }
  
  
  //only use this for programming strings to the eeprom
  void writeString(char *theString, int address) {
      //all strings are assumed to be 3 bytes long
      
      for(char i = 0; i < STRINGLENGTH; i++) {
          //Serial.print(theString[i]);
          i2c_eeprom_write_byte(0x50, address + i, theString[i]);
          delay(10);
      }
  }