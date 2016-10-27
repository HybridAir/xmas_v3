// handles low level eeprom storage stuff





    
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