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
int degrees{};
String name{};

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 11, en = 12, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Pototypes
Wind_dir find_dir(const volatile double &);
int wind_degree(const Wind_dir &);
String wind_name(const Wind_dir &);

void setup() {
  // Initalize the screen
  lcd.begin(16, 2);
}

void loop() {
  // value = analogRead(A0);
  // voltage = value * (5.0 / 1023);  // * ((R1 + R2) / R2);
  // // wind = find_dir(voltage);
  // // name = wind_name(wind);
  // // degrees = wind_degree(wind);
  // degrees = voltage * 95.745;

  // // Write the direction to screen
  // lcd.setCursor(0, 0);
  // lcd.print(voltage);
  // lcd.setCursor(0, 1);
  // lcd.print(degrees);
}

// Functions
double cal_wind_dir(analog_in_pin) {
  value = analogRead(analog_in_pin);
  voltage = value * (5.0 / 1023);  // * ((R1 + R2) / R2);
  // wind = find_dir(voltage);
  // name = wind_name(wind);
  // degrees = wind_degree(wind);
  degrees = voltage * 95.745;

  lcd.setCursor(0, 1);
  lcd.print(degrees);
  return degrees;
};

Wind_dir find_dir(const volatile double &volt) {
        if (volt >= 0.00 && volt < 0.47)  return Wind_dir::North;
  else  if (volt >= 0.47 && volt < 0.95)  return Wind_dir::Northeast;
  else  if (volt >= 0.95 && volt < 1.43)  return Wind_dir::East;
  else  if (volt >= 1.43 && volt < 1.90)  return Wind_dir::Southeast;
  else  if (volt >= 1.90 && volt < 2.38)  return Wind_dir::South;
  else  if (volt >= 2.38 && volt < 2.85)  return Wind_dir::Southwest;
  else  if (volt >= 2.85 && volt < 3.33)  return Wind_dir::West;
  else  if (volt >= 3.33 && volt < 4.00)  return Wind_dir::Northwest;
  else                                    return Wind_dir::Error;
};

int wind_degree(const Wind_dir &dir) {
  switch (dir) {
    case North:     return 0;
    case Northeast: return 45;
    case East:      return 90;
    case Southeast: return 135;
    case South:     return 180;
    case Southwest: return 225;
    case West:      return 270;
    case Northwest: return 315;
    case Error:     return -1;
    default:        return -1;
  }
}

String wind_name(const Wind_dir &dir) {
  switch (dir) {
    case North:     return "N    ";
    case Northeast: return "NE   ";
    case East:      return "E    ";
    case Southeast: return "SE   ";
    case South:     return "S    ";
    case Southwest: return "SW   ";
    case West:      return "W    ";
    case Northwest: return "NW   ";
    case Error:     return "Error";
    default:        return "Error";
  }
};