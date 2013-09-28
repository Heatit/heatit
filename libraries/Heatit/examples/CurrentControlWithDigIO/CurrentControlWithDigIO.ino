// Connect a switch to RX ( == D0 == 0) 
// when swi switched on, Heatit outputs 200mA on Ch0 for 1000ms

#include <Heatit.h> // call to the Heatit library

const int swi = RX; // == const int swi = 0;

void setup(){
  pinMode(swi, INPUT); 
} 

void loop(){

  if (digitalRead(swi == HIGH){
    Heatit.pins[0].setCurrent(400);
    delay(1000);  
  }
  Heatit.pins[0].setCurrent(0);
}
