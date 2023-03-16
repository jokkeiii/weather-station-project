/*
Print chars from A to Z, then resets.
Goes over all the cells like a snake.

The circuit:
 * LCD RS pin to digital pin 11
 * LCD Enable pin to digital pin 12
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
*/

#include <LiquidCrystal.h>

// Variables
char dispChar = 65;   // char A
#define DELAY = 150;  // ms

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 11, en = 12, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Prototypes
void snakePrint(const int &col, const int &row, char &disp);

// Setup
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
}

// Loop
void loop() {
  // Print dispChar to the LCD on top row.
  for (int i = 0; i < 16; i++)
    snakePrint(i, 0, dispChar);


  // Print dispChar to the LCD on bottom row.
  for (int i = 16; i > 0; i--)
    snakePrint(i, 1, dispChar);
}

// Prints chars from A to Z in snake pattern.
void snakePrint(const int &col, const int &row, char &disp) {
  // Clear screen, change position, display char
  lcd.clear();
  lcd.setCursor(col, row);
  lcd.print(disp);
  // Delay for char visibility
  delay(DELAY);
  // Roll display to next char
  disp++;
  if (disp > 90)  // char Z
    disp = 65;
}
