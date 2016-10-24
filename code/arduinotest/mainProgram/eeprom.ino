// handles lower level eeprom storage stuff





    
    void initEeprom() {
          //char somedata[18] = "BUTT STUFF"; // data to write
    Wire.begin(); // initialise the connection
    //i2c_eeprom_write_page(0x50, 0, (byte *)somedata, sizeof(somedata)); // write to EEPROM 
    //i2c_eeprom_write_page(0x50, 0, somedata, sizeof(somedata)); // write to EEPROM 

    //delay(10); //add a small delay
Serial.begin(9600);
    //Serial.println("Memory written");
    
    
     char string0[STRINGLENGTH] = "MERRY CHRISTMAS";
    char string1[STRINGLENGTH] = "HAPPY HOLIDAYS";
    char string2[STRINGLENGTH] = "HAPPY NEW YEAR";
    char string3[STRINGLENGTH] = "BY BLAKE STRATTON";
    char string4[STRINGLENGTH] = "XMAS_V3 2016";
    
    //i2c_eeprom_write_page(0x50, 0, string0, STRINGLENGTH);
    //delay(100);
    // i2c_eeprom_write_page(0x50, 0x20, string1, STRINGLENGTH);
    //delay(10);
    //i2c_eeprom_write_page(0x50, 0x24, string2, STRINGLENGTH);
   // delay(10);
    //i2c_eeprom_write_page(0x50, 0x36, string3, STRINGLENGTH);
    //delay(10);
    //i2c_eeprom_write_page(0x50, 0x48, string4, STRINGLENGTH);
    
     //writeString(string0, 0);
   // writeString(string1, 18);
     //writeString(string2, STRINGLENGTH * 2);
     //writeString(string3, STRINGLENGTH * 3);
    // writeString(string4, STRINGLENGTH * 4); 
    
    delay(10);
    
/*     for(char y = 54; y < 72; y++) {
            byte b = i2c_eeprom_read_byte(0x50, y);
            Serial.print((char)b); //print content to serial port
            Serial.print(".");
        } */
    
    
    //int addr=0; //first address
    //byte b = i2c_eeprom_read_byte(0x50, 18); // access the first address from the memory
    for(char i = 0; i <= 4; i++) {
        char stringAddress = 18 * i;
        //Serial.println(18 * i, DEC);
        for(char y = stringAddress; y < stringAddress + 18; y++) {
            byte b = i2c_eeprom_read_byte(0x50, y);
            //Serial.print((char)b); //print content to serial port
            //Serial.print(".");
            //Serial.print(y, DEC);
        }
        //Serial.println("");
    }
/*         while (addr < 32) 
        {
          Serial.print((char)b); //print content to serial port
          Serial.print(".");
          //Serial.println(addr, DEC);
          addr++; //increase address
          b = i2c_eeprom_read_byte(0x50, addr); //access an address from the memory
        } */
    }
    
    
    
    
    
//reads a character out of the eeprom, and writes it to the selected array position
void readChar(char theChar, char arrayNum) {
    //FIND WHERE THE CHARACTER LIVES
    //subtract 32 from the given character to get the character's 0-indexed position
    byte characterPos = theChar - MAP_START;
    
    //the character's address in eeprom starts at 0x0100, plus the character's position in ascii, times MAPSIZE (4)
    //so every 4 bytes, there is a character map
    byte lowAddr = LOW_CHARADDR;
    
    //set the lower address according to position in the 0-indexed ascii table
    lowAddr = LOW_CHARADDR + (characterPos * MAPSIZE);
    
    //combine both high and low addresses together to get your final address
    //int characterAddr = HI_CHARADDR << 8;
    //characterAddr = characterAddr | lowAddr;
    int characterAddr = (int)lowAddr;
    
    //prepare the array for storing the data we get from the eeprom
    //unsigned long input[MAPSIZE] = {0};
    unsigned long rawRead = 0;
    
    
    //for each byte in a character map storage area
    for(char i = 0; i < MAPSIZE; i++) {
        //shift the rawRead var over a byte to make room for the next read
        if(i > 0) {
            rawRead << 8;
        }
        
        //read the byte from the eeprom at the specified address and merge it into the rawRead var
        byte b = i2c_eeprom_read_byte(0x50, characterAddr + i);
        rawRead = rawRead | b;
        delay(10);
    }
    
    //get the width value out
    //shift the rawRead right by the number of bits in the character map storage area, minus 3 for the width value area
    unsigned long rawWidth = rawRead >> (MAPSIZE * 8) - 3;
    
/*     //compine the array into a single long
    unsigned long output = input[0] << 24;
    output = output | (input[1] << 16);
    output = output | (input[2] << 8);
    output = output | input[3]; */
    
    //return output;
    
    //get the width value out
    //unsigned long width = output >> (MAPSIZE * 8) - 3;
    //*charWidthOut = width * 5;
    
    
    //get the character map value out
    //unsigned long charMap = output & 0x1FFFFFFF;
    
    
    
    
        //byte i = 0;
    //stringIndex = 0;
    //while there is still physical room left for characters to be displayed (with 1 pixel space)
    //so if a character fits on the display and it's 1 px space fills the last column, dont get anymore
    //while(i < DISPLAY_WIDTH - 1) {
        
        
    //depending on the value of width, make a binary value the same length to use later
    //ie if the value of width is 4, then output 0b1111
    byte widthMerge = 0x1;
    for(int i = 0; i < rawWidth; i++) {
        widthMerge = (widthMerge << 0x1) | 0x1;
    }

        
    //for each row of the display (since all characters are DISPLAY_HEIGHT tall)
    //in ascending order because the lowest bit in the charMap is at the bottom of the map
    for(byte i = DISPLAY_HEIGHT; i > 0; i--) {
        //take out the leftmost width amount of bits, and put them in the current global character map variable
        charMaps[arrayNum][i] = rawRead & widthMerge;
        //move the local over width amount of bits
        rawRead = rawRead >> widthMerge;
        
    }
    
    //stringIndex++;
    //i = i + charWidths[stringIndex] + 1;
        
    //}
    
    
}
    
    
    
    
    
    
    
    
    
    
    




  void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
    int rdata = data;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    //Wire.write((eeaddress >> 8)); // MSB
    //Wire.write((eeaddress & 0xFF)); // LSB
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

  // WARNING: address is a page address, 6-bit end will wrap around
  // also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
  //void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, byte* data, byte length ) {
  void i2c_eeprom_write_page( byte deviceaddress, byte eeaddresspage, char *data, byte length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write(0x00);
    Wire.write(eeaddresspage);
    //Wire.write((int)(eeaddresspage >> 8)); // MSB
    //Wire.write((int)(eeaddresspage & 0xFF)); // LSB
    byte c;
    for ( c = 0; c < length; c++)
      Wire.write(data[c]);
    Wire.endTransmission();
  }



/*   // maybe let's not read more than 30 or 32 bytes at a time!
  void i2c_eeprom_read_buffer( int deviceaddress, unsigned int eeaddress, byte *buffer, int length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,length);
    int c = 0;
    for ( c = 0; c < length; c++ )
      if (Wire.available()) buffer[c] = Wire.read();
  } */