#include "HX711.h"



// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
const int Calibration_Weight = 1000;


HX711 scale;
void setup() {
 Serial.begin(9600);
 scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
 scale.set_scale();
 scale.tare();
 Serial.println("Calibration");
 Serial.println("Put a known weight on the scale");
 delay(5000);
 float x = scale.get_units(10);
 x = x / Calibration_Weight;
 scale.set_scale(x);
 Serial.println("Calibration finished...");
 delay(3000);
 }




void loop() {
 if (scale.is_ready()) {
 float reading = scale.get_units(10);
 //Serial.print("HX711 reading: ");
 Serial.println(reading);
 } 
 
 
 else {
 Serial.println("HX711 not found.");
 }
 delay(1000);

 //Serial.println("test");
}