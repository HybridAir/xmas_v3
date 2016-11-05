#ifndef _MAINPROGRAM_h_
#define _MAINPROGRAM_h_

//total number of leds in the display
#define NUM_LEDS 40

//display section names with their cooresponding charpliex pins
//F isn't used becuase AVR uses it for frequency stuff
#define A PA0
#define B PA1
#define C PA2
#define D PA3
#define E PA4
#define G PA5
#define H PA6

// where does our characterMap start in the ASCII code
#define MAP_START      32

//display size
#define DISPLAY_WIDTH  8
#define DISPLAY_HEIGHT 5

//the amount of charlieplexed display sections
#define NUM_SECTIONS 7

//character map width
#define CHAR_WIDTH 5

#define PREOFFSET 3

#define NUMOFSTRINGS 4           //4 strings saved in eeprom, 0 inclusive as usual
#define STRINGLENGTH 18          //largest string is 18 bytes long, so they all need to be

#endif