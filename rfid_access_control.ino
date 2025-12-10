#include <SPI.h>
#include <MFRC522.h>

// Configuration des pins
#define RST_PIN 9
#define SS_PIN 10
#define GREEN_LED 2
#define RED_LED 4
#define BUZZER 8

// Initialiser le module RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Variables pour gérer l'état
bool accessGranted = false;
unsigned long lastCardTime = 0;
const unsigned long CARD_COOLDOWN = 2000; // 2 secondes entre chaque scan

// IDs des cartes autorisées
const String HIBA_CARD_ID = "8B 8C ED 00";  // Hiba Riahi - ACTIF
const String OMAR_CARD_ID = "03 D1 10 2D";  // Omar Askri - EN CONGÉ

void setup() {
  // ============================================
  // CONFIGURATION INITIALE - Éteindre le buzzer
  // ============================================
  
  // Configurer le buzzer en OUTPUT IMMÉDIATEMENT
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
  
  // Configurer les LEDs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  
  // Attendre pour stabiliser
  delay(300);
  
  // Initialiser Serial
  Serial.begin(9600);
  delay(200);
  
  // FORCER le buzzer à LOW après Serial (sécurité maximale)
  for (int i = 0; i < 5; i++) {
    digitalWrite(BUZZER, LOW);
    delayMicroseconds(50);
  }
  
  // Initialiser SPI
  SPI.begin();
  
  // Initialiser RFID
  mfrc522.PCD_Init();
  delay(4);
  
  // Vérification finale - FORCER le buzzer à LOW
  for (int i = 0; i < 10; i++) {
    digitalWrite(BUZZER, LOW);
    delayMicroseconds(50);
  }
  delay(100);
  
  // Afficher la version du module RFID
  mfrc522.PCD_DumpVersionToSerial();
  
  Serial.println(F("=== SYSTEME DE CONTROLE D'ACCES RFID ==="));
  Serial.println(F("En attente de carte..."));
  Serial.println();
}

void loop() {
  // Vérifier si une nouvelle carte est présente
  if (!mfrc522.PICC_IsNewCardPresent()) {
    // Pas de carte - s'assurer que le buzzer est éteint
    digitalWrite(BUZZER, LOW);
    return;
  }
  
  // Lire l'UID de la carte
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  // Vérifier le cooldown pour éviter les scans multiples
  unsigned long currentTime = millis();
  if (currentTime - lastCardTime < CARD_COOLDOWN) {
    // Arrêter la communication avec la carte
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return;
  }
  lastCardTime = currentTime;
  
  // Convertir l'UID en String (format: "XX XX XX XX")
  String cardUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (i > 0) cardUID += " ";
    if (mfrc522.uid.uidByte[i] < 0x10) cardUID += "0";
    cardUID += String(mfrc522.uid.uidByte[i], HEX);
  }
  cardUID.toUpperCase();
  
  Serial.print(F("UID detecte: "));
  Serial.println(cardUID);
  
  // Vérifier quelle carte a été scannée
  if (cardUID == HIBA_CARD_ID) {
    // Carte de Hiba Riahi - Accès autorisé
    grantAccess("HIBA_RIAHI", "ACTIF");
  } else if (cardUID == OMAR_CARD_ID) {
    // Carte d'Omar Askri - Accès refusé (en congé)
    denyAccess("OMAR_ASKRI", "EN_CONGE", true);
  } else {
    // Carte inconnue - Accès refusé
    denyAccess("INCONNU", "CARTE_INCONNUE", true);
  }
  
  // Arrêter la communication avec la carte
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
  // Attendre un peu avant de permettre un nouveau scan
  delay(500);
}

void grantAccess(String employeeName, String status) {
  accessGranted = true;
  
  // Allumer la LED verte
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  
  // S'assurer que le buzzer est éteint
  digitalWrite(BUZZER, LOW);
  
  // Envoyer le message à l'application Qt
  // Format: ACCESS_GRANTED|EMPLOYEE_NAME|STATUS
  Serial.print("ACCESS_GRANTED|");
  Serial.print(employeeName);
  Serial.print("|");
  Serial.println(status);
  
  // Message sur le moniteur série
  Serial.println(F(">>> ACCES AUTORISE <<<"));
  Serial.print(F("Employe: "));
  Serial.println(employeeName);
  Serial.print(F("Statut: "));
  Serial.println(status);
  Serial.println(F("Bienvenue! Tu as l'acces pour la gestion pieces detachees"));
  Serial.println();
  
  // Maintenir la LED verte allumée pendant 3 secondes
  delay(3000);
  digitalWrite(GREEN_LED, LOW);
  
  accessGranted = false;
}

void denyAccess(String employeeName, String status, bool soundBuzzer) {
  accessGranted = false;
  
  // Allumer la LED rouge IMMÉDIATEMENT
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  
  // Activer le buzzer si demandé
  if (soundBuzzer) {
    // Sonner le buzzer 3 fois (code identique au test qui fonctionne)
    for (int i = 0; i < 3; i++) {
      digitalWrite(BUZZER, HIGH);
      delay(500);
      digitalWrite(BUZZER, LOW);
      delay(300);
    }
    Serial.println(F("ALERTE: Buzzer active - 3 bips sonnes!"));
  } else {
    // Pas de buzzer
    digitalWrite(BUZZER, LOW);
  }
  
  // Envoyer le message à l'application Qt
  // Format: ACCESS_DENIED|EMPLOYEE_NAME|STATUS
  Serial.print("ACCESS_DENIED|");
  Serial.print(employeeName);
  Serial.print("|");
  Serial.println(status);
  
  // Message sur le moniteur série
  Serial.println(F(">>> ACCES REFUSE <<<"));
  Serial.print(F("Employe: "));
  Serial.println(employeeName);
  Serial.print(F("Statut: "));
  Serial.println(status);
  if (soundBuzzer) {
    Serial.println(F("ALERTE: Carte non autorisee detectee!"));
  } else {
    Serial.println(F("Acces refuses - Gestion pieces detachees desactivee"));
  }
  Serial.println();
  
  // Maintenir la LED rouge allumée pendant 3 secondes
  delay(3000);
  digitalWrite(RED_LED, LOW);
}

