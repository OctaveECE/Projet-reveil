#include <LedControl.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

#define brocheDeSelection 10
#define brochePourLesDonnees 11
#define brochePourLhorloge 13
#define nombreDeMatricesLedRaccordees 4
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SW 7
#define CLK 5
#define DT 6

char menu1[] = "Modifier heure";
char menu2[] = "Mode affichage";
char menu3[] = "Retour";

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
int choix_du_menu = 1;

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

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Échec de l'initialisation de l'écran SSD1306"));
    while(1);
  }

  display.clearDisplay();
  display.display();

  // Initialisation des matrices LED
  for(int i = 0; i < nombreDeMatricesLedRaccordees; i++) {
    matriceLed.shutdown(i, false);
    matriceLed.setIntensity(i, 2);
    matriceLed.clearDisplay(i);
  }

  pinMode(SW, INPUT_PULLUP);
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(CLK), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DT), updateEncoder, CHANGE);
}

void afficherChiffreOuSymbole(byte indixMatrice, const byte* pointeurVersChiffreOuSymbole) {
  for (int ligne = 0; ligne < 8; ligne++) {
    matriceLed.setRow(indixMatrice, ligne, pgm_read_byte_near(pointeurVersChiffreOuSymbole + ligne));
  }
}

void ClignottementChaqueSeconde() {
  for(int i = 0; i < nombreDeMatricesLedRaccordees; i++) {
    matriceLed.setIntensity(i, 2);
  }
  delay(900);
  
  for(int i = 0; i < nombreDeMatricesLedRaccordees; i++) {
    matriceLed.setIntensity(i, 0);
  }
  delay(100);
}

void afficherHeure(int heure, int minute, bool mode24h) {
  if (!mode24h && heure > 12) {
    heure -= 12;
  }
  
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

void afficherMenu(int selection) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  const char* menus[] = {menu1, menu2, menu3};
  for(int i = 0; i < 3; i++) {
    if(i + 1 == selection) {
      display.fillRect(13, 17 + (i * 15), 70, 13, WHITE);
      display.setTextColor(BLACK);
    } else {
      display.setTextColor(WHITE);
    }
    display.setCursor(15, 20 + (i * 15));
    display.println(menus[i]);
  }
  display.display();
}

void modifierAffichageLED() {
  bool modificationEnCours = true;
  bool modifierHeures = true;
  DateTime now = rtc.now();
  int heureTemp = now.hour();
  int minuteTemp = now.minute();
  
  while(modificationEnCours) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println(modifierHeures ? "Modifier heures:" : "Modifier minutes:");
    
    display.setCursor(0, 20);
    display.setTextSize(2);
    char timeStr[6];
    sprintf(timeStr, "%02d:%02d", heureTemp, minuteTemp);
    display.println(timeStr);
    display.display();
    
    afficherHeure(heureTemp, minuteTemp, mode24Heures);
    
    if(digitalRead(CLK) != digitalRead(DT)) {
      if(modifierHeures) {
        heureTemp = (heureTemp + (digitalRead(CLK) == HIGH ? 1 : -1) + 24) % 24;
      } else {
        minuteTemp = (minuteTemp + (digitalRead(CLK) == HIGH ? 1 : -1) + 60) % 60;
      }
      delay(200);
    }
    
    if(digitalRead(SW) == LOW) {
      if(modifierHeures) {
        modifierHeures = false;
      } else {
        modificationEnCours = false;
        rtc.adjust(DateTime(now.year(), now.month(), now.day(), heureTemp, minuteTemp, 0));
      }
      delay(500);
    }
  }
}

void choisirModeAffichage() {
  bool selectionEnCours = true;
  bool choixActuel = mode24Heures;
  
  while(selectionEnCours) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    display.println("Mode:");
    display.setCursor(10, 40);
    display.println(choixActuel ? "-> 24h" : "   24h");
    display.setCursor(10, 50);
    display.println(choixActuel ? "   12h" : "-> 12h");
    display.display();
    
    if(digitalRead(CLK) != digitalRead(DT)) {
      choixActuel = !choixActuel;
      delay(200);
    }
    
    if(digitalRead(SW) == LOW) {
      mode24Heures = choixActuel;
      selectionEnCours = false;
      delay(500);
    }
  }
}

void loop() {
  DateTime now = rtc.now();
  int heure = now.hour();
  int minute = now.minute();
  
  if(digitalRead(SW) == LOW) {
    delay(200);  // Debounce
    
    bool menuActif = true;
    while(menuActif) {
      afficherMenu(choix_du_menu);
      
      if(digitalRead(CLK) != digitalRead(DT)) {
        choix_du_menu = ((choix_du_menu + (digitalRead(CLK) == HIGH ? 1 : -1) - 1 + 3) % 3) + 1;
        delay(200);
      }
      
      if(digitalRead(SW) == LOW) {
        delay(200);
        switch(choix_du_menu) {
          case 1:
            modifierAffichageLED();
            menuActif = false;
            break;
          case 2:
            choisirModeAffichage();
            menuActif = false;
            break;
          case 3:
            menuActif = false;
            break;
        }
      }
    }
  }
  
  afficherHeure(heure, minute, mode24Heures);
}
