#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


#define PIN_POMPE1 8
#define PIN_POMPE2 9
#define PIN_POTENTIOMETRE 1//A1
#define PIN_BP1 0x10
#define PIN_POIDS 2
#define PIN_LEDS 6
#define NUMPIXELS 10

Adafruit_NeoPixel strip(NUMPIXELS, PIN_LEDS, NEO_GRB + NEO_KHZ800);

void allumeLedsVert(){
  for (int iBcl=0;iBcl<NUMPIXELS;iBcl++){
      strip.setPixelColor(iBcl, strip.Color(0, 150, 0));
      strip.show();
    }
}

void allumeLedsBleu(){
  for (int iBcl=0;iBcl<NUMPIXELS;iBcl++){
      strip.setPixelColor(iBcl, strip.Color(0, 20, 150));
      strip.show();
    }
}

void allumeLedsLiq(int x_int_numLeds){
  for (int iBcl=0;iBcl<x_int_numLeds;iBcl++){
      strip.setPixelColor(iBcl, strip.Color(150, 0, 150));
      strip.show();
    }
}

void eteintLeds(){
  for (int iBcl=0;iBcl<NUMPIXELS;iBcl++){
      strip.setPixelColor(iBcl, strip.Color(0, 0, 0));
      strip.show();
    }
}

void allumePompe1(){
  digitalWrite(PIN_POMPE1, HIGH);
  Serial.println("Pompe 1 allumée");
}

void allumePompe2(){
  digitalWrite(PIN_POMPE2, HIGH);
  Serial.println("Pompe 2 allumée");
}

void eteintPompe1(){
  digitalWrite(PIN_POMPE1, LOW);
  Serial.println("Pompe 1 éteinte");
}

void eteintPompe2(){
  digitalWrite(PIN_POMPE2, LOW);
  Serial.println("Pompe 2 éteinte");
}