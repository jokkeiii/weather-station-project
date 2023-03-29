//    Alyk  jatko course 2020       24.3.2020 KN
//    Voltage and freq mode
//    Ethernet (1) module with W5100   / RED module

#include <LiquidCrystal.h>  // include LCD library
#include <Ethernet.h>       // include Ethernet libarty W5500 library

EthernetClient ethClient;                // Ethernet object var
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // LCD pin wiring settings fro NANO
#define mac_6 0x73                       //     Last mac number  MSB mac numbers at ethernet_mega.c
static uint8_t mymac[6] = { 0x44, 0x76, 0x58, 0x10, 0x00, mac_6 };
byte rev{};

void fetch_IP(void);

void setup() {
  // Serial.begin(9600);              // Serial monitor baudrate  = 9600
  lcd.begin(16, 2);                   // Display size defination 20 char  4 row
  lcd.setCursor(0, 0);                // set curosor to left upper corner
  lcd.print("24.3.2020 Alyk jatk ");  // print to lCD

  // get IP number from DHCP
  fetch_IP();  // initialize Ethernet connection
}

void loop() {}

// GET IP number from DHCP server
void fetch_IP(void) {
  rev = 1;
  lcd.setCursor(0, 1);
  lcd.print("Waiting IP");
  // delay(3000);
  rev = Ethernet.begin(mymac);  // get IP number

  if (rev == 0) {
    // Serial.print("We are here!");
    lcd.setCursor(0, 0);
    lcd.print("Ethernet failed ");
    return;
  }

  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("myIP=");
  // lcd.print("10.10.10.10");
  lcd.print(Ethernet.localIP());
  // delay(1500);
}
