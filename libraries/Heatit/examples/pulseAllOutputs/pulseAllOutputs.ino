// testing all the outputs, pusling them every 2 sec for 2 sec

#include <Heatit.h> // call to the Heatit library

void setup(){
  Heatit.setup(); // setup the Heatit library
  for (int i = 0; i < 6, i++){
    pinMode(i, OUTPUT); // easier way to set the regular IO-s as output
  }
} 

void loop(){
  for (int i = 0; i < 8; i++){
    Heatit.pins[i].setCurrent(300);
  }  
  for (int i = 0; i < 6, i++){
   digitalWrite(i, HIGH); // easier way to set the regular IO-s as output
  }
  delay(2000);
  
  for (int i = 0; i < 8; i++){
    Heatit.pins[i].setCurrent(0);
  }  
  for (int i = 0; i < 6, i++){
   digitalWrite(i, LOW); // easier way to set the regular IO-s as output
  }
  delay(2000);
}
