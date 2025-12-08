#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD address 0x27 (common), size 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {2,3,4,5};
byte colPins[COLS] = {6,7,8,9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String input = "";

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    if (key >= '0' && key <= '9') {
      // Add number to input
      input += key;
      lcd.setCursor(0,0);
      lcd.print(input);
    } 
    else if (key == 'A') {
  if (input.length() == 8) {
    Serial.println(input);   
    lcd.setCursor(0, 1);
    lcd.print("Sent        ");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Need 8 digits");
  }
}
    else if (key =  = 'C') {
      // Clear input and LCD
      input = "";
      lcd.clear();
    } 
    else if (key == 'D') {
      // Delete last character
      if (input.length() > 0) {
        input.remove(input.length() - 1);
        lcd.setCursor(0,0);
        lcd.print("                "); // clear line
        lcd.setCursor(0,0);
        lcd.print(input);
      }
    }
  }
}
