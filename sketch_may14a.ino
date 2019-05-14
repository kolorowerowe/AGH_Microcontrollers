#define trigPin 13
#define echoPin 12
#define offset 4

#include <TM1638.h>

TM1638 module(8, 9, 7);
byte displayDigits[] = {63,6,91,79,102,109,125,7,127,103 };
byte values[] = { 0,0,0,0,0,0,0,0 };
int theDigits[] = { 0,0,0,0 };
bool turn_on = false;


void setup() 
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  module.setDisplay(values);
  module.setupDisplay(true, 2); // where 7 is intensity (from 0 to 7)
  
}
void loop() 
{
  byte keys = module.getButtons();
  if(keys == 1)
    turn_on = true;
  if(keys == 2)
    turn_on = false;

  if (turn_on){
    double duration, distance;
    int distance_mm;
    digitalWrite(trigPin, LOW);        
    delayMicroseconds(2);              
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);           
    digitalWrite(trigPin, LOW);
    
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) / 2.91;
    distance_mm = (int) distance;
  
    delay(500);
    
    displayNumber(distance_mm);
  }
  else{
    displayNothing();
  }
  
}
void displayNothing(){
  
 for(int i = 0; i < 4; i++)
  values[i+offset] = 64;
 
 module.setDisplay(values);
 delay(200);
 
 for(int i = 0; i < 4; i++)
  values[i+offset] = 0;
  
 module.setDisplay(values);
   delay(200);
}
  

void displayNumber(int number_mm){
  
 // Extract the digits from this number.
 theDigits[0] = (int)(number_mm/1000);
 theDigits[1] = (int)((number_mm - (1000*theDigits[0])) / 100);
 theDigits[2] = (int)((number_mm - (1000*theDigits[0]) - (100*theDigits[1]))/10);
 theDigits[3] = (int)(number_mm - (1000*theDigits[0]) - (100*theDigits[1]) - (10*theDigits[2]));

 // Update the values in the values array used by the display.
 values[0+offset] = displayDigits[theDigits[0]];
 values[1+offset] = displayDigits[theDigits[1]]; 
 values[2+offset] = displayDigits[theDigits[2]] + 128;
 values[3+offset] = displayDigits[theDigits[3]];
 
 // Make sure that the number passed to the function was >= 0 or <100, otherwise show an error with ----.
 if(number_mm < 0 or number_mm > 9999){
    for(int i = 0; i < 4; i++)
      values[i+offset] = 64;
 } 
 
 // Update the display itself with the new values.
 module.setDisplay(values);
}
