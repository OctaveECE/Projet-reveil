#include <LedControl.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <RTClib.h>
#define brocheDeSelection               10        // Sortie D10 de l'Arduino (/SS)  vers la broche CS  de la matrice LED
#define brochePourLesDonnees            11        // Sortie D11 de l'Arduino (MOSI) vers la broche DIN de la matrice LED
#define brochePourLhorloge              13        // Sortie D13 de l'Arduino (SCK)  vers la broche CLK de la matrice LED
#define nombreDeMatricesLedRaccordees   4         // Nombre pouvant aller de 1 à 8 (nombre maxi de matrice led pilotable ici)
#define adresseDeLaMatrice              0
#define adresseDeLaMatrice1             1  // Pour les 8 matrices max évoquées ci-dessus, leurs index respectifs (adresses) vont de 0 à 7
#define adresseDeLaMatrice2             2
#define adresseDeLaMatrice3             3
#define delaiAllumageLed                100       // Temps de maintien d'allumage LED, exprimé en millisecondes
#define delaiEntreChaqueChangementAffichage  900
#define tempsDeClignottementChaqueSeconde 100


RTC_DS1307 rtc;
LedControl matriceLed = LedControl(brochePourLesDonnees, brochePourLhorloge, brocheDeSelection, nombreDeMatricesLedRaccordees);



const byte ETEINT[] PROGMEM = {
  0b00000000, 
  0b00000000,   // 2ème ligne de leds pour ce chiffre
  0b00000000,   // 3ème ligne de leds pour ce chiffre
  0b00000000,   // 4ème ligne de leds pour ce chiffre
  0b00000000,   // 5ème ligne de leds pour ce chiffre
  0b00000000,   // 6ème ligne de leds pour ce chiffre
  0b00000000,   // 7ème ligne de leds pour ce chiffre
  0b00000000    // 8ème ligne de leds pour ce chiffre
};

const byte CHIFFRE_0[] PROGMEM = {
  0b00111100, 
  0b01000010,   // 2ème ligne de leds pour ce chiffre
  0b01000010,   // 3ème ligne de leds pour ce chiffre
  0b01000010,   // 4ème ligne de leds pour ce chiffre
  0b01000010,   // 5ème ligne de leds pour ce chiffre
  0b01000010,   // 6ème ligne de leds pour ce chiffre
  0b01000010,   // 7ème ligne de leds pour ce chiffre
  0b00111100    // 8ème ligne de leds pour ce chiffre
};

const byte CHIFFRE_1[] PROGMEM = {
  0b00001000,   // 1ère ligne de leds pour ce chiffre
  0b00011000,   // 2ème ligne de leds pour ce chiffre
  0b00101000,   // 3ème ligne de leds pour ce chiffre
  0b00001000,   // 4ème ligne de leds pour ce chiffre
  0b00001000,   // 5ème ligne de leds pour ce chiffre
  0b00001000,   // 6ème ligne de leds pour ce chiffre
  0b00001000,   // 7ème ligne de leds pour ce chiffre
  0b00011100    // 8ème ligne de leds pour ce chiffre
};

const byte CHIFFRE_2[] PROGMEM = {
  0b00111100,   // 1ère ligne de leds pour ce chiffre
  0b01000010,   // 2ème ligne de leds pour ce chiffre
  0b01000010,   // 3ème ligne de leds pour ce chiffre
  0b00000100,   // 4ème ligne de leds pour ce chiffre
  0b00001000,   // 5ème ligne de leds pour ce chiffre
  0b00010000,   // 6ème ligne de leds pour ce chiffre
  0b00100000,   // 7ème ligne de leds pour ce chiffre
  0b01111110    // 8ème ligne de leds pour ce chiffre
};

const byte CHIFFRE_3[] PROGMEM = {
  0b01111110,   // 1ère ligne de leds pour ce chiffre
  0b00000100,   // 2ème ligne de leds pour ce chiffre
  0b00001000,   // 3ème ligne de leds pour ce chiffre
  0b00011100,   // 4ème ligne de leds pour ce chiffre
  0b00000010,   // 5ème ligne de leds pour ce chiffre
  0b00000010,   // 6ème ligne de leds pour ce chiffre
  0b01000010,   // 7ème ligne de leds pour ce chiffre
  0b00111100    // 8ème ligne de leds pour ce chiffre
};

const byte CHIFFRE_4[] PROGMEM = {
  0b00000100,   // 1ère ligne de leds pour ce chiffre
  0b00001100,   // 2ème ligne de leds pour ce chiffre
  0b00010100,   // 3ème ligne de leds pour ce chiffre
  0b00100100,   // 4ème ligne de leds pour ce chiffre
  0b01111111,   // 5ème ligne de leds pour ce chiffre
  0b00000100,   // 6ème ligne de leds pour ce chiffre
  0b00000100,   // 7ème ligne de leds pour ce chiffre
  0b00000100    // 8ème ligne de leds pour ce chiffre
};

const byte CHIFFRE_5[] PROGMEM = {
  0b01111110,   // 1ère ligne de leds pour ce chiffre
  0b01000000,   // 2ème ligne de leds pour ce chiffre
  0b01000000,   // 3ème ligne de leds pour ce chiffre
  0b01111100,   // 4ème ligne de leds pour ce chiffre
  0b00000010,   // 5ème ligne de leds pour ce chiffre
  0b00000010,   // 6ème ligne de leds pour ce chiffre
  0b01000010,   // 7ème ligne de leds pour ce chiffre
  0b00111100    // 8ème ligne de leds pour ce chiffre
};

const byte CHIFFRE_6[] PROGMEM = {
  0b00111100,   // 1ère ligne de leds pour ce chiffre
  0b01000010,   // 2ème ligne de leds pour ce chiffre
  0b01000000,   // 3ème ligne de leds pour ce chiffre
  0b01111100,   // 4ème ligne de leds pour ce chiffre
  0b01000010,   // 5ème ligne de leds pour ce chiffre
  0b01000010,   // 6ème ligne de leds pour ce chiffre
  0b01000010,   // 7ème ligne de leds pour ce chiffre
  0b00111100    // 8ème ligne de leds pour ce chiffre
};

const byte CHIFFRE_7[] PROGMEM = {
  0b01111110,   // 1ère ligne de leds pour ce chiffre
  0b00000010,   // 2ème ligne de leds pour ce chiffre
  0b00000010,   // 3ème ligne de leds pour ce chiffre
  0b00000100,   // 4ème ligne de leds pour ce chiffre
  0b00001000,   // 5ème ligne de leds pour ce chiffre
  0b00010000,   // 6ème ligne de leds pour ce chiffre
  0b00100000,   // 7ème ligne de leds pour ce chiffre
  0b01000000    // 8ème ligne de leds pour ce chiffre
};

const byte CHIFFRE_8[] PROGMEM = {
  0b00111100,   // 1ère ligne de leds pour ce chiffre
  0b01000010,   // 2ème ligne de leds pour ce chiffre
  0b01000010,   // 3ème ligne de leds pour ce chiffre
  0b00111100,   // 4ème ligne de leds pour ce chiffre
  0b01000010,   // 5ème ligne de leds pour ce chiffre
  0b01000010,   // 6ème ligne de leds pour ce chiffre
  0b01000010,   // 7ème ligne de leds pour ce chiffre
  0b00111100    // 8ème ligne de leds pour ce chiffre
};

const byte CHIFFRE_9[] PROGMEM = {
  0b00111100,   
  0b01000010,   
  0b01000010,  
  0b01000010,   
  0b00111110,   
  0b00000010,  
  0b01000010,   
  0b00111100    
};

const byte* ECRANETEINT = ETEINT;

const byte* CHIFFRE_[] = {
  CHIFFRE_0, CHIFFRE_1, CHIFFRE_2, CHIFFRE_3, CHIFFRE_4,
  CHIFFRE_5, CHIFFRE_6, CHIFFRE_7, CHIFFRE_8, CHIFFRE_9
};

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  
  if (!rtc.begin()) {
  Serial.println("Erreur de communication avec le module RTC");
  while (1);
}
rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

 
  matriceLed.shutdown(adresseDeLaMatrice, false); 
  matriceLed.shutdown(adresseDeLaMatrice1, false); 
  matriceLed.shutdown(adresseDeLaMatrice2, false); 
  matriceLed.shutdown(adresseDeLaMatrice3, false);       
  matriceLed.setIntensity(adresseDeLaMatrice, 5);
  matriceLed.setIntensity(adresseDeLaMatrice1, 5);
  matriceLed.setIntensity(adresseDeLaMatrice2, 5);
  matriceLed.setIntensity(adresseDeLaMatrice3, 5);       
  matriceLed.clearDisplay(adresseDeLaMatrice);   
  matriceLed.clearDisplay(adresseDeLaMatrice1);
  matriceLed.clearDisplay(adresseDeLaMatrice2);
  matriceLed.clearDisplay(adresseDeLaMatrice3);

  
}

void afficherChiffreOuSymbole(byte indixMatrice, byte* pointeurVersChiffreOuSymbole) {
  // Affiche le chiffre ou symbole sur une seule matrice à la position spécifiée
  for (int ligne = 0; ligne < 8; ligne++) {
    // Utilise la matrice correspondante pour afficher les données
    matriceLed.setRow(indixMatrice, ligne, pgm_read_byte_near(pointeurVersChiffreOuSymbole + ligne));
  }
}


void loop() {
 
DateTime now = rtc.now();

  // Récupérer l'heure et les minutes
  int heure = now.hour();
  int heure_Dizaines = heure / 10;
  int heure_Unite = heure % 10;
  int minute = now.minute();
  int minute_Dizaine = minute / 10;
  int minute_Unite = minute % 10;

  // Affichage des chiffres sur les matrices

  afficherChiffreOuSymbole(3, CHIFFRE_[heure_Dizaines]);   // 1ère matrice : dizaine de l'heure
  afficherChiffreOuSymbole(2, CHIFFRE_[heure_Unite]);      // 2ème matrice : unité de l'heure
  afficherChiffreOuSymbole(1, CHIFFRE_[minute_Dizaine]);   
  afficherChiffreOuSymbole(0, CHIFFRE_[minute_Unite]);     
  
  delay(delaiEntreChaqueChangementAffichage);  
  
  afficherChiffreOuSymbole(0,ECRANETEINT );   // 1ère matrice : dizaine de l'heure
  afficherChiffreOuSymbole(1,ECRANETEINT );      // 2ème matrice : unité de l'heure
  afficherChiffreOuSymbole(2,ECRANETEINT );   
  afficherChiffreOuSymbole(3,ECRANETEINT ); 

  delay(tempsDeClignottementChaqueSeconde);
}
