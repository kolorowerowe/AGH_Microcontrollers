#define trigPin 13
#define echoPin 12

#define offset 4
#define ile_prob 10

#include <TM1638.h>
TM1638 module(8, 9, 7); //number of pins to led&key

byte displayDigits[] = {63,6,91,79,102,109,125,7,127,103 };
byte values[] = { 0,0,0,0,0,0,0,0 };
int theDigits[] = { 0,0,0,0 };
bool turn_on = false;
bool clicked = false;

void setup() 
{
  //Serial.begin(9600); //for printing and debugging, might be deleted later
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  module.setDisplay(values);
  module.setupDisplay(true, 2); // where 7 is intensity (from 0 to 7)
}

void loop() 
{
  byte keys = module.getButtons();
  if(keys == 1){
    turn_on = true;
  }
  if(keys == 2){
    turn_on = false;
    clicked = false;
  }
  if (turn_on && !clicked){
    double duration[ile_prob], distance;
    int distance_mm;
    
    for (int i=0; i<ile_prob; i++){
      digitalWrite(trigPin, LOW);        
      delayMicroseconds(2);              
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);           
      digitalWrite(trigPin, LOW);
      duration[i] = pulseIn(echoPin, HIGH);
      delay(10);

    }

    
    double max_value = duration[0], min_value = duration[0], duration_mean = 0;
    int max_index = 0 , min_index = 0;
    
    for (int i = 1; i < ile_prob; i++){
      if (duration[i] >= max_value){
          max_value = duration[i];
          max_index = i;
        }
      else if (duration[i] <= min_value){
          min_value = duration[i];
          min_index = i;
        }
    }
    
    for (int i=0; i<ile_prob;i++){
        if (i!= min_index && i!= max_index){
          duration_mean+=duration[i];  
        }
    }


    duration_mean /= (ile_prob-2);
    distance = (duration_mean/2) / (29.1*0.1);
    distance_mm = (int) distance;

    delay(500);
    displayNumber(distance_mm);
    clicked = true;
  }
  else if (!clicked){
    displayNothing();
  }
}

void displayNothing(){
  
 for(int i = 0; i < 8; i++)
  values[i] = 64;
 
 module.setDisplay(values);
 delay(200);
 
 for(int i = 0; i < 8; i++)
  values[i] = 0;
  
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
