#include <LiquidCrystal.h>

// Variables
int angPin = analogRead(A1);

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 11, en = 12, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Functions
// TODO: ADC Reading = res 1023 / Sys Voltage 5V * Analog voltage

void setup() {
  // pinMode(input_pin, INPUT_PULLUP); 
  // attachInterrupt(digitalPinToInterrupt(input_pin), pulse_interrupt, RISING);

  // Initalize the screen
  lcd.begin(16, 2);
}

void loop() {
  // Write the counter to screen
  lcd.setCursor(0, 0);
  lcd.print(counter);
}
