#ifndef _MAINPROGRAM_h_
#define _MAINPROGRAM_h_


//GENERAL
#define DISPLAY_WIDTH  8
#define DISPLAY_HEIGHT 5


//ANIM
#define MAXSTRINGLENGTH
#define MAP_START      32           // where does our characterMap start in the ASCII code

#define TEXTSPEED 100
#define FIRSTDELAY  1000

#define NUMOFSTRINGS 4           //4 strings saved in eeprom, 0 inclusive as usual
#define MAXSTRINGLENGTH 18          //largest string is 18 bytes long, so they all need to be


//EEPROM
#define HI_STRINGADDR 0x00
#define LO_STRINGADDR 0x00

#define HI_CHARADDR 0x01        //starting high address for character maps
#define LOW_CHARADDR 0x00       //starting low address in eeprom for character maps
#define MAPSIZE 4              //the amount of bytes each character map entry uses in the eeprom


//DISPLAY
#define A 2
#define B 3
#define C 4
#define D 5
#define E 6
#define G 7
#define H 8

#define NUMOFLEDS 40



#endif