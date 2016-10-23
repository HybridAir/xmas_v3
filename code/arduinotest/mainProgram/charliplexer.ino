// handles low level display stuff
//TODO: display optimizations, test on 3 volts




// I skipped F because F isn't allowed
#define A 2
#define B 3
#define C 4
#define D 5
#define E 6
#define G 7
#define H 8

#define NUMOFLEDS 40

void initLeds() {
    pinMode(A, INPUT);
   pinMode(B, INPUT);
   pinMode(C, INPUT);
   pinMode(D, INPUT);
   pinMode(E, INPUT);
   pinMode(G, INPUT);
   pinMode(H, INPUT);
}


// visual array of how the pins are laid out
// first unit is the led cathode, second is anode
int ledGrid[NUMOFLEDS][2] = {

      {A,B},  {B,A},  {C,A},  {D,A},  {E,A},  {G,A},  {H,A},  {A,H},
      
      {A,C},  {B,C},  {C,B},  {D,B},  {E,B},  {G,B},  {H,B},  {B,H},

      {A,D},  {B,D},  {C,D},  {D,C},  {E,C},  {G,C},  {H,C},  {C,H},
      
      {A,E},  {B,E},  {C,E},  {D,E},  {E,D},  {G,D},  {H,D},  {D,H},

      {A,G},  {B,G},  {C,G},  {D,G},  {E,G},  {G,E},  {H,E},  {E,H}


};


// higher level sort of, ligths a pixel with x,y coords
void setPixel(byte x, byte y, boolean ledStatus) {
    if(ledStatus) {
        y = y - 1;
        x = x - 1;
        byte ledIndex = (y*DISPLAY_WIDTH) + x;
        setLed(ledGrid[ledIndex]);
    }
}




// lights a specific LED, needs the serialized led index (0 - 39)
void setLed(int pins[2]){
    
    // get the pins out of the requested led to be lit
  reset_pins();
    
  pinMode( pins[0], OUTPUT );
  digitalWrite( pins[0], LOW );
  
  pinMode( pins[1], OUTPUT );
  digitalWrite( pins[1], HIGH );
}

 
void reset_pins()
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
   
}