/*
Increment over a counter and display it on an LCD screen

The circuit:
 * Button to digital pin 2
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
const int inPin = 2;       // pushbutton connected to digital pin 2
volatile int counter = 0;  // variable to store the read value

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 11, en = 12, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Functions
void pulse_interrupt(void) {
  counter++;
}

void setup() {
  // Initialize input and attachInterrupt
  pinMode(inPin, INPUT_PULLUP);                                                   // Counting Pin 2 an input
  attachInterrupt(digitalPinToInterrupt(inPin), pulse_interrupt, RISING);  // Interrupt routine definition
  // Initalize the screen
  lcd.begin(16, 2);
}

void loop() {
  // Write the counter to screen
  lcd.setCursor(0, 0);
  lcd.print(counter);
}