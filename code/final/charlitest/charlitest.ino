//basic charlieplex matrix test, goes through all leds
#include <util/delay.h>



unsigned long previousMillis = 0;
const long interval = 100; // milliseconds gap between sending a network request.

// I skipped F because F isn't allowed
#define A PA0
#define B PA1
#define C PA2
#define D PA3
#define E PA4
#define G PA5
#define H PA6

const int count = 40;

int lit = 0;

// visual array of how the pins are laid out
int lights[count][2] = {

      {A,B},  {B,A},  {C,A},  {D,A},  {E,A},  {G,A},  {H,A},  {A,H},
      
      {A,C},  {B,C},  {C,B},  {D,B},  {E,B},  {G,B},  {H,B},  {B,H},

      {A,D},  {B,D},  {C,D},  {D,C},  {E,C},  {G,C},  {H,C},  {C,H},
      
      {A,E},  {B,E},  {C,E},  {D,E},  {E,D},  {G,D},  {H,D},  {D,H},

      {A,G},  {B,G},  {C,G},  {D,G},  {E,G},  {G,E},  {H,E},  {E,H}

};

void setup() {

/*    pinMode(A, INPUT);
   pinMode(B, INPUT);
   pinMode(C, INPUT);
   pinMode(D, INPUT);
   pinMode(E, INPUT);
   pinMode(G, INPUT);
   pinMode(H, INPUT);

   delay(1000); */
   // while(true) {
       // light(lights[39]); 
   // }
}



void loop() {

  for(int i=0;i<count;i++){
    //if(i == lit){
      light(lights[i]);
    //}
  }
    
  //unsigned long currentMillis = millis();

  //if (currentMillis - previousMillis >= interval) {
    //previousMillis = currentMillis;
/*     lit++;
    if(lit == count){
      lit = 0;
    } */
  //}

}

void light( int pins[2] ){

  //reset_pins();
  DDRA = 0;
  PORTA = 0;
    

  //pinMode( pins[0], OUTPUT );
  //digitalWrite( pins[0], LOW );
  
  
  //pinMode( pins[1], OUTPUT );
  //digitalWrite( pins[1], HIGH );
  
  DDRA |= (1<<pins[0]);
  DDRA |= (1<<pins[1]);
  PORTA |= (1<<pins[1]);
}

 
/* void reset_pins()
{
   pinMode(A, INPUT);
   pinMode(B, INPUT);
   pinMode(C, INPUT);
   pinMode(D, INPUT);
   pinMode(E, INPUT);
   pinMode(G, INPUT);
   pinMode(H, INPUT);
   digitalWrite(A, LOW);
   digitalWrite(B, LOW);
   digitalWrite(C, LOW);
   digitalWrite(D, LOW);
   digitalWrite(E, LOW);
   digitalWrite(G, LOW);
   digitalWrite(H, LOW);
   
} */
