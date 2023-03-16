/*
The circuit:
 * Analog read A4
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

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 11, en = 12, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Variables
#define BASE_A_READ 0
volatile unsigned long sensorValue{};
volatile unsigned long time{};
volatile unsigned long time_diff{};

int AD_conversion(byte ch) {
  DIDR0 = B11111111;
  ADMUX = B01000000 | ch;
  ADCSRA = B11000111;
  while ((ADCSRA & B01000000) != 0);
  return ADC;
}

// int AD_conversion(byte ch) {
//   DIDR0 = 0xFF;
//   ADMUX = (1 << 6) | ch;
//   ADCSRA = ((1 << 7) | (1 << 6) | 0x07);
//   while ((ADCSRA & (1 << 6)) != 0)    ;
//   return ADC;
// }

void setup() {

  // Initalize the screen
  // lcd.begin(16, 2);
  // lcd.setCursor(0, 0);
}

// lcd.print(sensorValue);
void loop() {
#if BASE_A_READ
  sensorValue = analogRead(A4);
  time = micros();
  Serial.println("\nSensorvalue: ");
  Serial.println(sensorValue);
  time_diff = micros() - time;
  Serial.println("\nTime diff: ");
  Serial.println(time_diff);
#else
  sensorValue = AD_conversion(A4);
  time = micros();
  Serial.println("\nSensorvalue: ");
  Serial.println(sensorValue);
  time_diff = micros() - time;
  Serial.println("\nTime diff: ");
  Serial.println(time_diff);
#endif
}