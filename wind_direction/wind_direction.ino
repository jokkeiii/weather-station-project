/*
Calculate and show input voltage
Determine and show input data wind direction

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

// Wind direction
enum Wind_dir {
  North,
  Northeast,
  East,
  Southeast,
  South,
  Southwest,
  West,
  Northwest,
  Error,
};

// Variables
volatile double value{};
volatile double voltage{};
Wind_dir wind{};
String name{};

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 11, en = 12, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Pototypes
Wind_dir find_dir(const volatile double &);
String wind_name(const Wind_dir &);

void setup() {
  // Initalize the screen
  lcd.begin(16, 2);
}

void loop() {
  value = analogRead(A0);
  voltage = value * (5.0 / 1023);  // * ((R1 + R2) / R2);
  wind = find_dir(voltage);
  name = wind_name(wind);

  // Write the direction to screen
  lcd.setCursor(0, 0);
  lcd.print(voltage);
  lcd.setCursor(0, 1);
  lcd.print(name);
}

// Functions
Wind_dir find_dir(const volatile double &volt) {
        if (volt >= 0.00 && volt < 0.47)  return Wind_dir::North;
  else  if (volt >= 0.47 && volt < 0.95)  return Wind_dir::Northeast;
  else  if (volt >= 0.95 && volt < 1.43)  return Wind_dir::East;
  else  if (volt >= 1.43 && volt < 1.90)  return Wind_dir::Southeast;
  else  if (volt >= 1.90 && volt < 2.38)  return Wind_dir::South;
  else  if (volt >= 2.38 && volt < 2.85)  return Wind_dir::Southwest;
  else  if (volt >= 2.85 && volt < 3.33)  return Wind_dir::West;
  else  if (volt >= 3.33)                 return Wind_dir::Northwest;
  else                                    return Wind_dir::Error;
};

String wind_name(const Wind_dir &dir) {
  switch (dir) {
    case North:     return "North";
    case Northeast: return "Northeast";
    case East:      return "East";
    case Southeast: return "Southeast";
    case South:     return "South";
    case Southwest: return "Southwest";
    case West:      return "West";
    case Northwest: return "Northwest";
    case Error:     return "Error";
    default:        return "Error";
  }
};