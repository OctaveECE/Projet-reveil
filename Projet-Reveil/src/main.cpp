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

int choixMenu = 1;
int choixSousMenu = 1;

char menu1[] = "Reglages Heure";
char menu2[] = "Reglages Reveil";
char menu3[] = "Retour";

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
  display.fillRect(0, 0, 128, 64, WHITE);
  display.display();
  delay(500);
  Serial.flush();
}

void testUnitaireEncodeur(){
  Serial.begin(9600);
  Serial.println(encoderValue);
  delay(100);
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




const char* mainMenu[] = {"Reglages Heure", "Reglages Reveil", "Retour"};
const char* sousMenHeure[] = {"Changer Heure", "24H / 12H", "Retour"};
const char* sousMenReveil[] = {"Alarme", "Sonnerie", "Retour"};
const char* sousMenAlarme[] = {"Heure Alarme", "Alarme ON/OFF", "Retour"};
const char* sousMenSonnerie[] = {"Choix Sonnerie", "Regler Snooze", "Retour"};

int currentMenuLevel = 0;  // 0 = main menu, 1 = first submenu, 2 = second submenu
int currentMainMenuIndex = 0;
int currentHeureMenuIndex = 0;
int currentReveilMenuIndex = 0;
int currentAlarmeMenuIndex = 0;
int currentSonnerieMenuIndex = 0;

void displayMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  switch (currentMenuLevel) {
    case 0:  // Main Menu
      for (int i = 0; i < 3; i++) {
        display.setCursor(15, 20 + (i * 15));
        if (i == currentMainMenuIndex) {
          display.fillRect(13, 17 + (i * 15), 100, 13, WHITE);
          display.setTextColor(BLACK);
        } else {
          display.setTextColor(WHITE);
        }
        display.println(mainMenu[i]);
      }
      break;

    case 1:  // First Submenu
      if (currentMainMenuIndex == 0) {  // Reglages Heure
        for (int i = 0; i < 3; i++) {
          display.setCursor(15, 20 + (i * 15));
          if (i == currentHeureMenuIndex) {
            display.fillRect(13, 17 + (i * 15), 100, 13, WHITE);
            display.setTextColor(BLACK);
          } else {
            display.setTextColor(WHITE);
          }
          display.println(sousMenHeure[i]);
        }
      } else if (currentMainMenuIndex == 1) {  // Reglages Reveil
        for (int i = 0; i < 3; i++) {
          display.setCursor(15, 20 + (i * 15));
          if (i == currentReveilMenuIndex) {
            display.fillRect(13, 17 + (i * 15), 100, 13, WHITE);
            display.setTextColor(BLACK);
          } else {
            display.setTextColor(WHITE);
          }
          display.println(sousMenReveil[i]);
        }
      }
      break;

    case 2:  // Second Submenu
      if (currentMainMenuIndex == 1) {  // Reglages Reveil
        if (currentReveilMenuIndex == 0) {  // Alarme
          for (int i = 0; i < 3; i++) {
            display.setCursor(15, 20 + (i * 15));
            if (i == currentAlarmeMenuIndex) {
              display.fillRect(13, 17 + (i * 15), 100, 13, WHITE);
              display.setTextColor(BLACK);
            } else {
              display.setTextColor(WHITE);
            }
            display.println(sousMenAlarme[i]);
          }
        } else if (currentReveilMenuIndex == 1) {  // Sonnerie
          for (int i = 0; i < 3; i++) {
            display.setCursor(15, 20 + (i * 15));
            if (i == currentSonnerieMenuIndex) {
              display.fillRect(13, 17 + (i * 15), 100, 13, WHITE);
              display.setTextColor(BLACK);
            } else {
              display.setTextColor(WHITE);
            }
            display.println(sousMenSonnerie[i]);
          }
        }
      }
      break;
  }
  
  display.display();
}

void navigateMenu() {
  int valeurSW = digitalRead(SW);
  
  // Button is pressed (LOW)
  if (valeurSW == LOW) {
    delay(200);  // Debounce
    
    // Select option
    switch (currentMenuLevel) {
      case 0:  // Main Menu
        if (currentMainMenuIndex == 2) {  // Retour
          return;
        }
        currentMenuLevel = 1;
        break;
      
      case 1:  // First Submenu
        if (currentMainMenuIndex == 0) {  // Reglages Heure
          if (currentHeureMenuIndex == 2) {  // Retour
            currentMenuLevel = 0;
            return;
          }
          switch (currentHeureMenuIndex) {
            case 0:  // Changer Heure
            
              break;
            case 1:  // 24H / 12H
              mode24h12h = !mode24h12h;
              break;
          }
        } else if (currentMainMenuIndex == 1) {  // Reglages Reveil
          if (currentReveilMenuIndex == 2) {  // Retour
            currentMenuLevel = 0;
            return;
          }
          currentMenuLevel = 2;
        }
        break;
      
      case 2:  // Second Submenu
        if (currentMainMenuIndex == 1) {  // Reglages Reveil
          if (currentReveilMenuIndex == 0) {  // Alarme
            if (currentAlarmeMenuIndex == 2) {  // Retour
              currentMenuLevel = 1;
              return;
            }
            switch (currentAlarmeMenuIndex) {
              case 0:  // Heure Alarme
                // Implement alarm hour setting
                break;
              case 1:  // Alarme ON/OFF
                // Implement alarm toggle
                break;
            }
          } else if (currentReveilMenuIndex == 1) {  // Sonnerie
            if (currentSonnerieMenuIndex == 2) {  // Retour
              currentMenuLevel = 1;
              return;
            }
            switch (currentSonnerieMenuIndex) {
              case 0:  // Choix Sonnerie
                // Implement melody selection
                break;
              case 1:  // Regler Snooze
                // Implement snooze setting
                break;
            }
          }
        }
        break;
    }
    
    displayMenu();
    return;
  }

  // Check encoder rotation
  if (encoderValue > 0) {
    switch (currentMenuLevel) {
      case 0:
        currentMainMenuIndex = (currentMainMenuIndex + 1) % 3;
        break;
      case 1:
        if (currentMainMenuIndex == 0) {
          currentHeureMenuIndex = (currentHeureMenuIndex + 1) % 3;
        } else if (currentMainMenuIndex == 1) {
          currentReveilMenuIndex = (currentReveilMenuIndex + 1) % 3;
        }
        break;
      case 2:
        if (currentReveilMenuIndex == 0) {
          currentAlarmeMenuIndex = (currentAlarmeMenuIndex + 1) % 3;
        } else if (currentReveilMenuIndex == 1) {
          currentSonnerieMenuIndex = (currentSonnerieMenuIndex + 1) % 3;
        }
        break;
    }
    encoderValue = 0;
    displayMenu();
  } else if (encoderValue < 0) {
    switch (currentMenuLevel) {
      case 0:
        currentMainMenuIndex = (currentMainMenuIndex - 1 + 3) % 3;
        break;
      case 1:
        if (currentMainMenuIndex == 0) {
          currentHeureMenuIndex = (currentHeureMenuIndex - 1 + 3) % 3;
        } else if (currentMainMenuIndex == 1) {
          currentReveilMenuIndex = (currentReveilMenuIndex - 1 + 3) % 3;
        }
        break;
      case 2:
        if (currentReveilMenuIndex == 0) {
          currentAlarmeMenuIndex = (currentAlarmeMenuIndex - 1 + 3) % 3;
        } else if (currentReveilMenuIndex == 1) {
          currentSonnerieMenuIndex = (currentSonnerieMenuIndex - 1 + 3) % 3;
        }
        break;
    }
    encoderValue = 0;
    displayMenu();
  }
}

void selectionMenu() {
  static unsigned long lastDisplayTime = 0;
  
  // Only update display periodically to avoid flickering
  if (millis() - lastDisplayTime > 200) {
    displayMenu();
    lastDisplayTime = millis();
  }
  
  navigateMenu();
}

void loop() {
  DateTime now = rtc.now();
  int heure = now.hour();
  int minute = now.minute();
  
  
  afficher24h(heure, minute);
  
  selectionMenu();
  
}
