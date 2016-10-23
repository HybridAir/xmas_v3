//writes strings and stuff to the eeprom, mainly used for testing


#include <Wire.h> //I2C library

#define STRINGLENGTH 18


void setup() {
    Serial.begin(9600);
    Wire.begin();
    
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
    
    for(char i = 0; i <= 4; i++) {
        char stringAddress = 18 * i;
        //Serial.println(18 * i, DEC);
        for(char y = stringAddress; y < stringAddress + 18; y++) {
            byte b = i2c_eeprom_read_byte(0x50, y);
            Serial.print((char)b); //print content to serial port
            //Serial.print(".");
            //Serial.print(y, DEC);
        }
        
    }
    
    Serial.println("Done");

}


void loop() {
    
    
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