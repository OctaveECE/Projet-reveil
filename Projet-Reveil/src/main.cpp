#include <LedControl.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <Ultrasonic.h>

#define brocheDeSelection 10
#define brochePourLesDonnees 11
#define brochePourLhorloge 13
#define nombreDeMatricesLedRaccordees 4
#define adresseDeLaMatrice 0
#define adresseDeLaMatrice1 1
#define adresseDeLaMatrice2 2
#define adresseDeLaMatrice3 3
#define delaiAllumageLed 100
#define delaiEntreChaqueChangementAffichage 900
#define tempsDeClignottementChaqueSeconde 100
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define buzzer 0
#define boutonpin 2
#define SW 7
#define CLK 5
#define DT 6
#define BUZZER_PIN 7
#define NOTE_DO  262
#define NOTE_RE  294
#define NOTE_MI  330
#define NOTE_FA  349
#define NOTE_SOL 392
#define NOTE_LA  440
#define NOTE_SI  494
#define NOTE_DO2 523

char menu1[] = "Test menu 1";
char menu2[] = "Test menu 2";
char menu3[] = "Test menu 3";

RTC_DS1307 rtc;
LedControl matriceLed = LedControl(brochePourLesDonnees, brochePourLhorloge, brocheDeSelection, nombreDeMatricesLedRaccordees);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const byte ETEINT[] PROGMEM = {
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000
};

const byte CHIFFRE_0[] PROGMEM = {
  0b00111100, 0b01000010, 0b01000010, 0b01000010,
  0b01000010, 0b01000010, 0b01000010, 0b00111100
};

const byte CHIFFRE_1[] PROGMEM = {
  0b00001000, 0b00011000, 0b00101000, 0b00001000,
  0b00001000, 0b00001000, 0b00001000, 0b00011100
};

const byte CHIFFRE_2[] PROGMEM = {
  0b00111100, 0b01000010, 0b01000010, 0b00000100,
  0b00001000, 0b00010000, 0b00100000, 0b01111110
};

const byte CHIFFRE_3[] PROGMEM = {
  0b01111110, 0b00000100, 0b00001000, 0b00011100,
  0b00000010, 0b00000010, 0b01000010, 0b00111100
};

const byte CHIFFRE_4[] PROGMEM = {
  0b00000100, 0b00001100, 0b00010100, 0b00100100,
  0b01111111, 0b00000100, 0b00000100, 0b00000100
};

const byte CHIFFRE_5[] PROGMEM = {
  0b01111110, 0b01000000, 0b01000000, 0b01111100,
  0b00000010, 0b00000010, 0b01000010, 0b00111100
};

const byte CHIFFRE_6[] PROGMEM = {
  0b00111100, 0b01000010, 0b01000000, 0b01111100,
  0b01000010, 0b01000010, 0b01000010, 0b00111100
};

const byte CHIFFRE_7[] PROGMEM = {
  0b01111110, 0b00000010, 0b00000010, 0b00000100,
  0b00001000, 0b00010000, 0b00100000, 0b01000000
};

const byte CHIFFRE_8[] PROGMEM = {
  0b00111100, 0b01000010, 0b01000010, 0b00111100,
  0b01000010, 0b01000010, 0b01000010, 0b00111100
};

const byte CHIFFRE_9[] PROGMEM = {
  0b00111100, 0b01000010, 0b01000010, 0b01000010,
  0b00111110, 0b00000010, 0b01000010, 0b00111100
};

const byte* ECRANETEINT = ETEINT;

const byte* CHIFFRE_[] = {
  CHIFFRE_0, CHIFFRE_1, CHIFFRE_2, CHIFFRE_3, CHIFFRE_4,
  CHIFFRE_5, CHIFFRE_6, CHIFFRE_7, CHIFFRE_8, CHIFFRE_9
};

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

bool mode24Heures = true;
// Déclaration de la fonction updateEncoder
void updateEncoder();

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

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // remet le module rtc a l'heure actuelle
  char menu1[] = "Test menu 1";
  char menu2[] = "Test menu 2";
  char menu3[] = "Test menu 3";
  int heureALARM;
  int minuteALARM;

  int melodie[] = {
  NOTE_DO, NOTE_RE, NOTE_MI, NOTE_FA, NOTE_SOL, NOTE_LA, NOTE_SI, NOTE_DO2
};

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

  pinMode(SW, INPUT_PULLUP);
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(CLK), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DT), updateEncoder, CHANGE);
}

void updateEncoder() {
  int MSB = digitalRead(CLK);
  int LSB = digitalRead(DT);

  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded << 2) | encoded;

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue--;

  lastEncoded = encoded;
}

void afficherChiffreOuSymbole(byte indixMatrice, byte* pointeurVersChiffreOuSymbole) {
  // Affiche le chiffre ou symbole sur une seule matrice à la position spécifiée
  for (int ligne = 0; ligne < 8; ligne++) {
    // Utilise la matrice correspondante pour afficher les données
    matriceLed.setRow(indixMatrice, ligne, pgm_read_byte_near(pointeurVersChiffreOuSymbole + ligne));
  }
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
  int heure_Dizaines = (heure > 12 ? heure - 12 : heure) / 10;
  int heure_Unite = (heure > 12 ? heure - 12 : heure) % 10;
  int minute_Dizaine = minute / 10;
  int minute_Unite = minute % 10;

  afficherChiffreOuSymbole(3, CHIFFRE_[heure_Dizaines]);
  afficherChiffreOuSymbole(2, CHIFFRE_[heure_Unite]);
  afficherChiffreOuSymbole(1, CHIFFRE_[minute_Dizaine]);
  afficherChiffreOuSymbole(0, CHIFFRE_[minute_Unite]);

  ClignottementChaqueSeconde();
}

void changerHeureLED(int* heure, int* minute){
  int valeurSW = digitalRead(SW);
  bool changerHeures = true;

  while (valeurSW == HIGH) {
    int valeurCLK = digitalRead(CLK);
    int valeurDT = digitalRead(DT);
    if (valeurCLK != valeurDT) {
      if (changerHeures) {
        *heure = *heure + 1;
        if (*heure >= 24) {
          *heure = 0;
        }
      } else {
        *minute = *minute + 1;
        if (*minute >= 60) {
          *minute = 0;
        }
      }
      if (mode24Heures) {
        afficher24h(*heure, *minute);
      } else {
        afficher12h(*heure, *minute);
      }
    }
    valeurSW = digitalRead(SW);
    if (valeurSW == LOW) {
      changerHeures = !changerHeures;
      delay(500); 
      valeurSW = digitalRead(SW);
    }
  }

  rtc.adjust(DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), *heure, *minute, 0)); // Ajuste l'heure de la RTC
}
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
  display.display();
}

void menuSecondChoix(){
  display.clearDisplay();
  display.fillRect(13, 32, 70, 13, WHITE);
  display.setTextColor(BLACK);
  display.setCursor(15, 35);
  display.println(menu2);
  display.display();
}

void menuTroisiemeChoix(){
  display.clearDisplay();
  display.fillRect(13, 47, 70, 13, WHITE);
  display.setTextColor(BLACK);
  display.setCursor(15, 50);
  display.println(menu3);
  display.display();
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
  
}

void testUnitaireLCD(){
  afficherChiffreOuSymbole(3, ECRANETEINT);  
  afficherChiffreOuSymbole(2, ECRANETEINT);     
  afficherChiffreOuSymbole(1, ECRANETEINT);   
  afficherChiffreOuSymbole(0, ECRANETEINT);
  delay(100);
}
//void jouerAalarm() {
  //for (int i = 0; i < sizeof(melodie) / sizeof(melodie[0]); i++) {
   // tone(BUZZER_PIN, melodie[i], 400);  
    //noTone(BUZZER_PIN);        
 // }}
void choisirModeAffichage() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.println("Choisir le mode d'affichage:");
  display.setCursor(10, 40);
  display.println("1. 24 heures");
  display.setCursor(10, 50);
  display.println("2. 12 heures");
  display.display();

  int valeurSW = digitalRead(SW);
  bool modeChoisi = false;
  while (!modeChoisi) {
    int valeurCLK = digitalRead(CLK);
    int valeurDT = digitalRead(DT);
    if (valeurCLK != valeurDT) {
      mode24Heures = !mode24Heures;
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 20);
      display.println("Choisir le mode d'affichage:");
      display.setCursor(10, 40);
      if (mode24Heures) {
        display.println("-> 24 heures");
        display.setCursor(10, 50);
        display.println("   12 heures");
      } else {
        display.println("    24 heures");
        display.setCursor(10, 50);
        display.println("-> 12 heures");
      }
      display.display();
    }
    valeurSW = digitalRead(SW);
    if (valeurSW == LOW) {
      modeChoisi = true;
      delay(500); // Debounce delay
    }
  }
}
void reveilmatin(int* heureReveil, int* minuteReveil){
int valeurSW = digitalRead(SW);
bool changerHeures = true;

while (valeurSW == HIGH) {
int valeurCLK = digitalRead(CLK);
int valeurDT = digitalRead(DT);
if (valeurCLK != valeurDT) {
if (changerHeures) {
*heureReveil = *heureReveil + 1;
if (*heureReveil >= 24) {
*heureReveil = 0;
}
} else {
*minuteReveil = *minuteReveil + 1;
if (*minuteReveil >= 60) {
*minuteReveil = 0;
}
}

// Afficher l'heure de réveil sur l'écran OLED en temps réel
display.clearDisplay();
display.setTextSize(2);
display.setTextColor(WHITE);
display.setCursor(10, 20);
display.print("Reveil: ");
display.print(*heureReveil);
display.print(":");
if (*minuteReveil < 10) {
display.print("0");
}
display.print(*minuteReveil);
display.display();
}
valeurSW = digitalRead(SW);
if (valeurSW == LOW) {
changerHeures = !changerHeures;
delay(500); // Debounce delay
valeurSW = digitalRead(SW);
}
}
}
void menu(int* heure, int* minute){
  int choix_du_menu = 1;
  int valeurSW = digitalRead(SW);
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
    valeurSW = digitalRead(SW);
  }

  if (choix_du_menu == 1) {
    changerHeureLED(heure, minute);
  }
  if (choix_du_menu == 2) {
    choisirModeAffichage();
  }
  if (choix_du_menu == 3) {
      afficher12h(heure,minute);
    //reveilmatin(&heure, &minute);
  }
}


void loop() {
  DateTime now = rtc.now();
  // Récupérer l'heure et les minutes
  int heure = now.hour();
  int minute = now.minute();
int valeurSW = digitalRead(SW);
    if (mode24Heures) {
    afficher24h(heure, minute);
  } else {
    afficher12h(heure, minute);
  }
  if (valeurSW == LOW){
  menu(&heure, &minute);}

   testUnitaireRTC(heure, minute);
}

