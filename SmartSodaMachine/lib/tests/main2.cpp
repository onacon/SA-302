#include "myHX711.h"



// HX711 circuit wiring
const int data = 2;
const int clk = 3;
const int Calibration_Weight = 1000;


HX711 scale;
void setup() {
 Serial.begin(9600);
 scale.begin(128);//check
 scale.set_scale(1.f);// check
 scale.tare(10);//check
 Serial.println("Calibration");
 Serial.println("Put a known weight on the scale");
 delay(5000);
 float x = scale.get_units(10);
 Serial.println(x);
 x = x / Calibration_Weight;
 scale.set_scale(x);//check
 Serial.println("Calibration finished...");
 delay(3000);
 }




void loop() {
 if (scale.is_ready()) {//check
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