/*

The circuit:
 * Pulse input to pin 2
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

#include <TimerOne.h>
#include <LiquidCrystal.h>

// Variables
const int pulse_input_pin = 2;
volatile byte time = 0;  // Introduction of global variable time
volatile int pulse = 0;
int freq = 0;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 11, en = 12, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Functions
void timer_int_routine() {  // Interrupt routine, called every 1/2 sec
  time++;                   // Increment over time every 1/2 sec
  if (time > 9) {
    time = 0;          // reset time every 5 sec
    freq = pulse / 5;  // Save pulse to value to freq, pulse count is 5 sec
    pulse = 0;         // Reset pulse
  }
}

void pulse_interrupt(void) {
  pulse++;
}

void setup() {
  Timer1.initialize(500000);                  // Initialize timer1, and set a 500 000 uS = 1/2 second period
  Timer1.attachInterrupt(timer_int_routine);  // Attach timer_int_routine() as a timer overflow

  pinMode(pulse_input_pin, INPUT);                                                   // Counting Pin 2 an input
  attachInterrupt(digitalPinToInterrupt(pulse_input_pin), pulse_interrupt, RISING);  // Interrupt routine definition

  // Initalize the screen
  lcd.begin(16, 2);
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print(freq);
}
