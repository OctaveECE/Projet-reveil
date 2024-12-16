#include <LedControl.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
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
#define tempsDeClignottementChaqueSeconde    100
#define SCREEN_WIDTH 128 // Largeur de l'écran OLED, en pixels
#define SCREEN_HEIGHT 64 // Hauteur de l'écran OLED, en pixels
#define BUZZER_PIN 7
#define SW  7
#define CLK 5
#define DT  2
#define NOTE_DO  262
#define NOTE_RE  294
#define NOTE_MI  330
#define NOTE_FA  349
#define NOTE_SOL 392
#define NOTE_LA  440
#define NOTE_SI  494
#define NOTE_DO2 523


RTC_DS1307 rtc;
LedControl matriceLed = LedControl(brochePourLesDonnees, brochePourLhorloge, brocheDeSelection, nombreDeMatricesLedRaccordees);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


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

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

int choix_du_menu = 1;

char menu1[] = "Changer Heures";
char menu2[] = "Conversion";
char menu3[] = "Test menu 3";

int heureALARM;
int minuteALARM;
int mode24h12h = 0;

int melodie[] = {
  NOTE_DO, NOTE_RE, NOTE_MI, NOTE_FA, NOTE_SOL, NOTE_LA, NOTE_SI, NOTE_DO2
}; 

void updateEncoder() {
  int MSB = digitalRead(CLK);
  int LSB = digitalRead(DT);

  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded << 2) | encoded;

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue--;

  lastEncoded = encoded;
}

void setup() {
  Serial.begin(9600);
  if (!rtc.begin()) {
  Serial.println("Erreur de communication avec le module RTC");
  while (1);
  }
  Serial.flush(); //permet d'utiliser un deuxieme Serial.begin

  Serial.begin(115200); // Initialisation de la communication série
    // Initialisation de l'écran OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
  Serial.println(F("Échec de l'initialisation de l'écran SSD1306"));
  while(1); // Boucle infinie en cas d'échec
  }
  Serial.flush();

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // remet le module rtc a l'heure actuelle 
  
  pinMode(SW, INPUT_PULLUP);
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(CLK), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DT), updateEncoder, CHANGE);

  display.clearDisplay();
 
  matriceLed.shutdown(adresseDeLaMatrice, false); 
  matriceLed.shutdown(adresseDeLaMatrice1, false); 
  matriceLed.shutdown(adresseDeLaMatrice2, false); 
  matriceLed.shutdown(adresseDeLaMatrice3, false);       
  matriceLed.setIntensity(adresseDeLaMatrice, 2);
  matriceLed.setIntensity(adresseDeLaMatrice1, 2);
  matriceLed.setIntensity(adresseDeLaMatrice2, 2);
  matriceLed.setIntensity(adresseDeLaMatrice3, 2);       
  matriceLed.clearDisplay(adresseDeLaMatrice);   
  matriceLed.clearDisplay(adresseDeLaMatrice1);
  matriceLed.clearDisplay(adresseDeLaMatrice2);
  matriceLed.clearDisplay(adresseDeLaMatrice3);

  Serial.begin(9600);
}

void afficherChiffreOuSymbole(byte indixMatrice, byte* pointeurVersChiffreOuSymbole) {
  // Affiche le chiffre ou symbole sur une seule matrice à la position spécifiée
  for (int ligne = 0; ligne < 8; ligne++) {
    // Utilise la matrice correspondante pour afficher les données
    matriceLed.setRow(indixMatrice, ligne, pgm_read_byte_near(pointeurVersChiffreOuSymbole + ligne));
  }
}


void testUnitaireRTC(int heure, int minute){
  Serial.begin(9600);
  Serial.print(heure);
  Serial.println(minute);
  delay(100);
  Serial.flush();
}

void testUnitaireOLED(){
  Serial.begin(115200);
  display.fillRect(0, 64, 128, 64, WHITE);
  delay(500);
  Serial.flush();
}

void testUnitaireLCD(){
  afficherChiffreOuSymbole(3, ECRANETEINT);  
  afficherChiffreOuSymbole(2, ECRANETEINT);     
  afficherChiffreOuSymbole(1, ECRANETEINT);   
  afficherChiffreOuSymbole(0, ECRANETEINT);
  delay(100);
}

void ClignottementChaqueSeconde(){
  matriceLed.setIntensity(adresseDeLaMatrice, 2);
  matriceLed.setIntensity(adresseDeLaMatrice1, 2);
  matriceLed.setIntensity(adresseDeLaMatrice2, 2);
  matriceLed.setIntensity(adresseDeLaMatrice3, 2); 

  delay(delaiEntreChaqueChangementAffichage);  
  
  matriceLed.setIntensity(adresseDeLaMatrice, 0);
  matriceLed.setIntensity(adresseDeLaMatrice1, 0);
  matriceLed.setIntensity(adresseDeLaMatrice2, 0);
  matriceLed.setIntensity(adresseDeLaMatrice3, 0);

  delay(tempsDeClignottementChaqueSeconde);
}

void afficher24h(int heure, int minute){
  
  int heure_Dizaines = heure / 10;
  int heure_Unite = heure % 10;
  int minute_Dizaine = minute / 10;
  int minute_Unite = minute % 10;

  afficherChiffreOuSymbole(3, CHIFFRE_[heure_Dizaines]);  
  afficherChiffreOuSymbole(2, CHIFFRE_[heure_Unite]);     
  afficherChiffreOuSymbole(1, CHIFFRE_[minute_Dizaine]);   
  afficherChiffreOuSymbole(0, CHIFFRE_[minute_Unite]);     
  
  ClignottementChaqueSeconde();

}

void afficher12h(int heure, int minute){

  int heure_Dizaines = (heure > 12? heure-12 : heure) / 10;
  int heure_Unite = (heure > 12? heure-12 : heure) % 10;
  int minute_Dizaine = minute / 10;
  int minute_Unite = minute % 10;
  
  afficherChiffreOuSymbole(3, CHIFFRE_[heure_Dizaines]);  
  afficherChiffreOuSymbole(2, CHIFFRE_[heure_Unite]);     
  afficherChiffreOuSymbole(1, CHIFFRE_[minute_Dizaine]);   
  afficherChiffreOuSymbole(0, CHIFFRE_[minute_Unite]);

  ClignottementChaqueSeconde();

}

void changerHeureLED(int heure, int minute){
  
  /*ajouter code qui modifie nouvelleHeure et nouvelleMinute en fonction
  de l'encodeur rotatif*/ 
  rtc.adjust(DateTime((heure*3600)+(minute*60)));
}

void mode12h24h(int mode24h12h, int heure, int minute){
  if (mode24h12h == 0){
    afficher24h(heure, minute);
  } 
  else afficher12h(heure, minute);
}

/*
void jouerAalarm() {
  for (int i = 0; i < sizeof(melodie) / sizeof(melodie[0]); i++) {
    tone(BUZZER_PIN, melodie[i], 400);  
    noTone(BUZZER_PIN);        
  }
}
*/
void menuSansSelection(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(15, 20);
  display.println(menu1);
  display.setTextColor(WHITE);
  display.setCursor(15, 35);
  display.println(menu2);
  display.setTextColor(WHITE);
  display.setCursor(15, 50);
  display.println(menu3);
  display.display();
}

void menuPremierChoix(){
  display.clearDisplay();
  display.fillRect(13, 17, 70, 13, WHITE);
  display.setTextColor(BLACK);
  display.setCursor(15, 20);
  display.println(menu1);
  display.setTextColor(WHITE);
  display.setCursor(15, 35);
  display.println(menu2);
  display.setTextColor(WHITE);
  display.setCursor(15, 50);
  display.println(menu3);
  display.display();
}

void menuSecondChoix(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(15, 20);
  display.println(menu1);
  display.fillRect(13, 32, 70, 13, WHITE);
  display.setTextColor(BLACK);
  display.setCursor(15, 35);
  display.println(menu2);
  display.setTextColor(WHITE);
  display.setCursor(15, 50);
  display.println(menu3);
  display.display();
}

void menuTroisiemeChoix(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(15, 20);
  display.println(menu1);
  display.setTextColor(WHITE);
  display.setCursor(15, 35);
  display.println(menu2);
  display.fillRect(13, 47, 70, 13, WHITE);
  display.setTextColor(BLACK);
  display.setCursor(15, 50);
  display.println(menu3);
  display.display();
}

void testUnitaireEncodeur(){
  Serial.println(encoderValue);
  delay(100);
}

void menu(int choix_du_menu){
  int valeurSW = digitalRead(SW);
  int valeurDT = digitalRead(DT);
  int valeurCLK = digitalRead(CLK);
  while (valeurSW == HIGH) {
    int valeurDT = digitalRead(DT);
    int valeurCLK = digitalRead(CLK);
    if (valeurCLK != valeurDT) {
      choix_du_menu++;
      if (choix_du_menu > 3) {
        choix_du_menu = 1;
      }
      if (choix_du_menu < 1) {
        choix_du_menu = 3;
      }
    }
    switch (choix_du_menu) {
      case 1:
        menuPremierChoix();
        break;
      case 2:
        menuSecondChoix();
        break;
      case 3:
        menuTroisiemeChoix();
        break;
    }
  }
}

void loop() {
  
  DateTime now = rtc.now();
  // Récupérer l'heure et les minutes
  int heure = now.hour();
  int minute = now.minute();
  
  
  afficher24h(heure, minute);
  menu(choix_du_menu);
  testUnitaireEncodeur();
}
