#include <Arduino.h>
#include <HX711.h>
#include <Adafruit_NeoPixel.h>
#include <fonctiondantoinelapute.h>
/*
#define PIN_POMPE1 8
#define PIN_POMPE2 9
#define PIN_POTENTIOMETRE 1//A1
#define PIN_BP1 0x10
#define PIN_POIDS 2
#define PIN_LEDS 6
#define NUMPIXELS 10
*/
long g_int_poids;
int g_int_dose_liq1 = 400;
int g_int_dose_liq2 = 200;
int g_int_tare = 50;
bool g_bool_BP;
bool g_bool_etatPompe1;
bool g_bool_etatPompe2;
const int LOADCELL_DOUT_PIN = 3; // Pin de données du capteur HX711
const int LOADCELL_SCK_PIN = 2;  // Pin d'horloge du capteur HX711
const int WEIGHT_THRESHOLD = 1000; // Seuil de poids en grammes
const int Calibration_Weight = 1000;


HX711 scale;


void weightInterrupt() {
    long weight = scale.get_units(10); // Lire le poids (moyenne sur 10 lectures pour la stabilité)
    g_int_poids = weight;
    if (weight > WEIGHT_THRESHOLD) {
        Serial.print("Poids detecte : ");
        Serial.print(weight);
        Serial.println(" grammes - Poids superieur a 1500 grammes!");
        
        // Ajoutez ici le code à exécuter lorsque le poids dépasse 200 grammes
    }

}





typedef enum {READY,ATT_BP,SERVICE1,SERVICE2,FIN} TEtat;


TEtat g_etat = READY;




void setup() {
    Serial.begin(9600);
    DDRB = 0x00;
    DDRC = 0x00;
     DDRD = 0x00;
    pinMode(PIN_POMPE1, OUTPUT);
    pinMode(PIN_POMPE2, OUTPUT);
    strip.begin();
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.set_scale();
    scale.tare();
    Serial.println("Calibration");
    Serial.println("Put a known weight on the scale");
    delay(5000);
    float x = scale.get_units(10);
    Serial.println(x);
    x = x / Calibration_Weight;
    scale.set_scale(x);
    Serial.println("Calibration finished...");
    delay(3000);
    attachInterrupt(digitalPinToInterrupt(LOADCELL_DOUT_PIN), weightInterrupt, FALLING);
}


void loop() {
//VARIABLES LOCALES
  int l_int_numLeds;


  //LECTURE ENTREES

  //Etat du bouton cablé sur PORT Dx = PDx
  g_bool_BP = !((PIND & PIN_BP1) == PIN_BP1);

  //debug poids
  //g_int_poids = analogRead(PIN_POTENTIOMETRE);



  //MAJ ETAT
  switch(g_etat){
    case READY :
      if(g_bool_BP==true){
        g_etat = ATT_BP; 
      }
      break; 
    case ATT_BP :
      if(g_bool_BP==false){
        g_etat = SERVICE1; 
      }
      break;
    case SERVICE1 :
      if(g_int_poids >= g_int_tare + g_int_dose_liq1){
        g_etat = SERVICE2;
      }
      break;
    case SERVICE2 : 
      if(g_int_poids >= g_int_tare + g_int_dose_liq1 + g_int_dose_liq2){
        g_etat = FIN;
      }
      break;
    case FIN :
      if(g_int_poids<g_int_tare){
        g_etat = READY;
      }
  }


  //MAJ SORTIES
  switch(g_etat){
    case READY :
      g_bool_etatPompe1 = false;
      g_bool_etatPompe2 = false;
      allumeLedsBleu();
      break;
    
    case ATT_BP :
      eteintLeds();
      break;
    
    case SERVICE1 :
      //LED MAPS
      l_int_numLeds = map(g_int_poids, g_int_tare, g_int_tare + g_int_dose_liq1 + g_int_dose_liq2, 0, NUMPIXELS);
      //Serial.print("Num leds : ");
      //Serial.println(l_int_numLeds);
      
      allumeLedsLiq(l_int_numLeds);
      g_bool_etatPompe1 = true;
      //allumePompe1();

      g_bool_etatPompe2 = false;
      //eteintPompe2();

      break;
    
    case SERVICE2 :
      //LED MAPS
      l_int_numLeds = map(g_int_poids, g_int_tare, g_int_tare + g_int_dose_liq1 + g_int_dose_liq2, 0, NUMPIXELS);
      allumeLedsLiq(l_int_numLeds);

      g_bool_etatPompe1 = false;
      //eteintPompe1();

      g_bool_etatPompe2 = true;
      //allumePompe2();
      break;
    
    case FIN :
      g_bool_etatPompe1 = false;
      //eteintPompe1();

      g_bool_etatPompe2 = false;
      //eteintPompe2();

      allumeLedsVert();
      break;

    default :
      break;
  }

  if(g_bool_etatPompe1==false){
    eteintPompe1();
  }
  if(g_bool_etatPompe2==false){
    eteintPompe2();
  }
  if(g_bool_etatPompe1==true){
    allumePompe1();
  }
  if(g_bool_etatPompe2==true){
    allumePompe2();
  }

//DEBUG
  Serial.print("Etat : ");
  Serial.println(g_etat);
  Serial.print("Poids : ");
  Serial.println(g_int_poids);
  delay(500);
}



