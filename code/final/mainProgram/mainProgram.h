#ifndef _MAINPROGRAM_h_
#define _MAINPROGRAM_h_

#define BUTTON PB2

//DISPLAY

//display section names with their cooresponding charpliex pins
//F isn't used becuase AVR uses it for frequency stuff
#define A PA0
#define B PA1
#define C PA2
#define D PA3
#define E PA4
#define G PA5
#define H PA6

//total number of leds in the display
#define NUM_LEDS 40

#define DISPLAY_WIDTH  8
#define DISPLAY_HEIGHT 5

#define NUM_SECTIONS 7
#define PREOFFSET 3


//ANIM
#define MAP_START      32

#define CHAR_WIDTH 5

#define TOTAL_STRINGS 5           
#define STRINGLENGTH 18          //largest string is 18 bytes long, so they all need to be

#define TEXTSPEED 100
#define FIRSTDELAY  1000

#endif