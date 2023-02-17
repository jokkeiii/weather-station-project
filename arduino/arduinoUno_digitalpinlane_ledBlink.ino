// C++ code
//
/*
  This program blinks pin 13 of the Arduino (the
  built-in LED)
*/

void setup()
{
  // 	  76543210
  DDRD = B11111111;
}

void loop()
{
  PORTD = B11111111;
  delay(1000);
  PORTD = B00000000;
  delay(1000); // Wait for 1000 millisecond(s)
  
  PORTD = B10000000;
  delay(100);
  PORTD = B01000000;
  delay(100);
  PORTD = B00100000;
  delay(100);
  PORTD = B00010000;
  delay(100);
  PORTD = B00001000;
  delay(100);
  PORTD = B00000100;
  delay(100);
  PORTD = B00000010;
  delay(100);
  PORTD = B00000001;
  delay(100);
  PORTD = B00000000;
  delay(500);
}