/* In the following  example the CH0 on the Heatit board increases the output amprege
  setp by step
*/

#include <Heatit.h> // call to the Heatit library

void setup() {
  Heatit.setup(); // setup the Heatit library
}

void loop() {
  Heatit.pins[0].setCurrent(0);
  delay(1000);
  Heatit.pins[0].setCurrent(100);
  delay(1000);
  Heatit.pins[0].setCurrent(200);
  delay(1000);
  Heatit.pins[0].setCurrent(300);
  delay(1000);
  Heatit.pins[0].setCurrent(400);
  delay(1000);

}
