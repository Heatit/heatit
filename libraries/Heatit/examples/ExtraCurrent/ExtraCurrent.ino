// You may apply more current when several Heatit channels are connected
// For example, we will use CH0 and CH1 to apply 600mA

#include <Heatit.h> // call to the Heatit library

void setup(){
  Heatit.setup(); // setup the Heatit library
} 

void loop(){
  Heatit.pins[0].setCurrent(300);
  Heatit.pins[1].setCurrent(300);
  delay(1000);
  Heatit.pins[0].setCurrent(0);
  Heatit.pins[1].setCurrent(0);
  delay(1000);
}
