/*
Print chars from A to Z, then resets.
Goes over all the cells like a snake.

For Arduine UNO!! Change for Arduino NANO.
The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/

#include <LiquidCrystal.h>

// Vars
int seconds = 0;
char dispChar = 65; // A
const int DELAY = 150; // ms
// Prototype
void snakePrint(const int &col, const int &row, char &disp, const int &dly);

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
// For Arduine UNO!! Change for Arduino NANO.
const int rs = 12, en = 11, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Setup
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();

}

// Loop
void loop() {
  // Print dispChar to the LCD on top row.
  for (int i = 0; i <= 15; i++) {
    snakePrint(i, 0, dispChar, DELAY);
  }

  // Print dispChar to the LCD on bottom row.
  for (int i = 15; i >= -1; i--) {
    snakePrint(i, 1, dispChar, DELAY);
  }
}

// Prints chars from A to Z in snake pattern.
void snakePrint(const int &col, const int &row, char &disp, const int &dly) {
  // Clear screen, change position, display char
  lcd.clear();
  lcd.setCursor(col, row);
  lcd.print(disp);
  // Delay for char visibility
  delay(dly);
  // Roll display to next char 
  disp++;
  if (disp > 90) // Z
    disp = 65;
}
