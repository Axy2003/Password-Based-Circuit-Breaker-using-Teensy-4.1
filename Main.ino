#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define RELAY_PIN 13  // Relay pin to control the circuit breaker
#define PASSWORD "1234"  // Set your password here

// Initialize the LCD (assuming address 0x27 and size 16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad setup
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; // Connect to row pins
byte colPins[COLS] = {5, 4, 3, 2}; // Connect to column pins

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String enteredPassword = "";

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  // digitalWrite(RELAY_PIN, LOW); // Start with the relay off

  // Initialize I2C communication
  Wire.begin();
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    // If '#' is pressed, check the entered password
    if (key == '#') {
      if (enteredPassword == PASSWORD) {
        // Correct password
        if (digitalRead(RELAY_PIN) == HIGH){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Access Granted");
          digitalWrite(RELAY_PIN, LOW);  // Turn on the relay
          delay(2000);  // Wait 2 seconds for relay to engage
          lcd.clear();
          lcd.print("Breaker OFF");
          delay(2000);  // Wait 2 seconds for relay to engage
          lcd.clear();
          lcd.print("Enter Password:");
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Access Granted");
          digitalWrite(RELAY_PIN, HIGH);  // Turn on the relay
          delay(2000);  // Wait 2 seconds for relay to engage
          lcd.clear();
          lcd.print("Breaker ON");
          delay(2000);  // Wait 2 seconds for relay to engage
          lcd.clear();
          lcd.print("Enter Password:");
        }
      } else {
        // Incorrect password
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access Denied");
        // digitalWrite(RELAY_PIN, LOW);  // Keep relay off
        delay(2000);
        lcd.clear();
        lcd.print("Enter Password:");
      }
      enteredPassword = "";  // Clear entered password after checking
    }
    else if (key == 'D') {
      // Clear the entered password if '*' is pressed
      enteredPassword = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Password:");
    }
    else {
      // Append the entered key to the password
      enteredPassword += key;
      lcd.setCursor(0, 1);
      lcd.print(enteredPassword);
    }
  }
}
